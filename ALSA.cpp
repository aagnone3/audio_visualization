//
// Created by aagnone3 on 8/17/16.
//

#include "ALSA.hpp"

ALSA::ALSA()
  : AudioInput()
{
  requestedSamplingRate = 44100;
  nChannels = 2;
  /* set for 16-bit */
  bytesPerFrame = 2*nChannels;
  /* // 735 = 44100Hz/60fps assumed */
  framesPerPeriod = (unsigned int) (requestedSamplingRate/60.0);
  /* must be >=2, see ALSA manual for details */
  nPeriods = 2;
  bufferMemorySeconds = 20.0;

  periodLengthBytes = framesPerPeriod*bytesPerFrame;
  audioBufferChunk = new char[periodLengthBytes];
  requestedBufferSizeFrames = framesPerPeriod*nPeriods;
  bufferIndex = 0;
  /* sets up sound card for recording, e.g. sets samplingRate, bufferSizeFrames, etc.
   * if the device fails to initialize, we must exit and fix the problem. */
  if (initDevice()<0)
    exit(1);
  samplingPeriod = 1.0f/samplingRate;
  bufferSizeSamples = (int) (bufferMemorySeconds*samplingRate);
  audioBuffer = new float[bufferSizeSamples];

  /* start recording thread to independently write data into the audio buffer */
  captureThread = std::make_unique<std::thread>(audioCapture, (void*) this);
}

ALSA::~ALSA() {
  snd_pcm_close(pcmHandle);
  fftwf_destroy_plan(fftPlan);
  captureThread->join();
  delete[] audioBufferChunk;
  delete[] audioBuffer;
  delete[] spectrogramSlice;
  delete[] windowedAudioFrame;
  delete[] windowingFunction;
}

void ALSA::audioCapture(void* alsaInputInstance)
{
  // still mostly Luke's code, some names changed. Aims to read 1 "period"
  // (ALSA device setting) into the current write index of our ai->b buffer.
  Log::getInstance()->logger() << "audioCapture thread started" << std::endl;
  // shares data with the main thread
  ALSA* alsaInput = (ALSA*) alsaInputInstance;

  float inv256 = 1.0f/256.0f;
  float inv256_2 = inv256*inv256;

  /* loop until the state of AudioInput kills it */
  while (!alsaInput->quit) {
    snd_pcm_sframes_t n;
    if (!alsaInput->pause) {
      /* keep trying to get exactly 1 "period" of raw data from sound card */
      n = snd_pcm_readi(alsaInput->pcmHandle, alsaInput->audioBufferChunk, alsaInput->framesPerPeriod);
      while (n<0) {
        Log::getInstance()->logger() << "Error occured while recording: " << snd_strerror(n) << std::endl;
        //n = snd_pcm_recover(ai->pcmHandle, n, 0); // ahb
        snd_pcm_prepare(alsaInput->pcmHandle);
        Log::getInstance()->logger() << "Dropped audio data (frames read n=" << n << std::endl;
        n = snd_pcm_readi(alsaInput->pcmHandle, alsaInput->audioBufferChunk, alsaInput->framesPerPeriod);
      }
      /* now we have n samples */
      //if (verbosity>=1) printf("snd_pcm_readi got n=%d frames\n", n);

      byte by;
      int write_ptr, read_ptr;
      /* read a chunk from the audio buffer  */
      for (int i = 0; i<n; i++) { // read audioBufferChunk into our buffer ai->b ...
        read_ptr = i*alsaInput->bytesPerFrame;
        write_ptr = mod(alsaInput->bufferIndex+i, alsaInput->bufferSizeSamples); // wraps around
        by.signedVal = alsaInput->audioBufferChunk[read_ptr];
        // compute float in [-1/2,1/2) from 16-bit raw... (LSB unsigned char)
        alsaInput->audioBuffer[write_ptr] = (float) alsaInput->audioBufferChunk[read_ptr+1]*inv256+(float) by.unsignedVal*inv256_2;
      }
      alsaInput->bufferIndex = mod(alsaInput->bufferIndex+n, alsaInput->bufferSizeSamples);  // update index (in one go)
      computeSpectrogramSlice(alsaInput);
    }
    else {
      usleep(10000);  // wait 0.01 sec if paused (keeps thread CPU usage low)
    }
  }
  fprintf(stderr, "audioCapture thread exiting.\n");
}

int ALSA::initDevice()
{
  streamDirection = SND_PCM_STREAM_CAPTURE;
  /* Init pcmDeviceName. Of course, later you */
  /* will make this configurable ;-)     */
  pcmDeviceName = strdup("plughw:0,0");
  /* Allocate the snd_pcm_hw_params_t structure on the stack. */
  snd_pcm_hw_params_alloca(&hardwareParams);
  /* Open PCM. The last parameter of this function is the mode. */
  /* If this is set to 0, the standard mode is used. Possible   */
  /* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */
  /* If SND_PCM_NONBLOCK is used, read / write access to the    */
  /* PCM device will return immediately. If SND_PCM_ASYNC is    */
  /* specified, SIGIO will be emitted whenever a period has     */
  /* been completely processed by the soundcard.                */
  if (snd_pcm_open(&pcmHandle, pcmDeviceName, streamDirection, 0)<0) {
    fprintf(stderr, "Error opening PCM device %s\n", pcmDeviceName);
    return (-1);
  }
  /* Init hardwareParams with full configuration space */
  if (snd_pcm_hw_params_any(pcmHandle, hardwareParams)<0) {
    fprintf(stderr, "Can not configure this PCM device.\n");
    return (-1);
  }
  /* Set access type. This can be either    */
  /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
  /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
  /* There are also access types for MMAPed */
  /* access, but this is beyond the scope   */
  /* of this introduction.                  */
  if (snd_pcm_hw_params_set_access(pcmHandle, hardwareParams, SND_PCM_ACCESS_RW_INTERLEAVED)<0) {
    fprintf(stderr, "Error setting access.\n");
    return (-1);
  }

  /* Set sample format */
  if (snd_pcm_hw_params_set_format(pcmHandle, hardwareParams, SND_PCM_FORMAT_S16_LE)<0) {
    fprintf(stderr, "Error setting format.\n");
    return (-1);
  }

  /* Set sample samplingRate. If the requested samplingRate is not supported */
  /* by the hardware, use nearest possible samplingRate.         */
  samplingRate = requestedSamplingRate;
  if (snd_pcm_hw_params_set_rate_near(pcmHandle, hardwareParams, (uint*) &samplingRate, 0)<0) {
    fprintf(stderr, "Error setting samplingRate.\n");
    return (-1);
  }
  if (samplingRate!=requestedSamplingRate) {
    fprintf(stderr, "The samplingRate %d Hz is not supported by your hardware.\n \
                        ==> Using %d Hz instead.\n", requestedSamplingRate, samplingRate);
  }

  /* report the comparison of samplingRate and requestedSamplingRate */
  if (samplingRate == requestedSamplingRate) {
    dir = 0;
  } else if (samplingRate > requestedSamplingRate) {
    dir = 1;
  } else {
    dir = -1;
  }

  /* Set number of nChannels */
  if (snd_pcm_hw_params_set_channels(pcmHandle, hardwareParams, nChannels)<0) {
    fprintf(stderr, "Error setting nChannels.\n");
    return (-1);
  }

  /* Set number of periods. Periods used to be called fragments. */
  if (snd_pcm_hw_params_set_periods(pcmHandle, hardwareParams, nPeriods, 0)<0) {
    fprintf(stderr, "Error setting number of periods.\n");
    return (-1);
  }
  /* Set buffer size (in frames). The resulting latency is given by */
  /* latency = periodLengthBytes * nPeriods / (samplingRate * bytesPerFrame)     */
  bufferSizeFrames = requestedBufferSizeFrames;
  if (snd_pcm_hw_params_set_buffer_size_near(pcmHandle, hardwareParams, &bufferSizeFrames)<0) {
    fprintf(stderr, "Error setting buffersize.\n");
    return (-1);
  }
  if (bufferSizeFrames!=requestedBufferSizeFrames) {
    fprintf(stderr, "Buffer bufferSizeFrames %d is not supported, using %d instead.\n", (int) requestedBufferSizeFrames,
        (int) bufferSizeFrames);
  }

  /* Apply HW parameter settings to PCM device and prepare device  */
  if (snd_pcm_hw_params(pcmHandle, hardwareParams)<0) {
    fprintf(stderr, "Error setting HW params.\n");
    return (-1);
  }
  return 1;
}

void ALSA::quitNow()
{
  quit = true;
  //      pthread_kill_other_threads_np();
  snd_pcm_close(pcmHandle);
}

snd_pcm_uframes_t ALSA::getPeriodLengthBytes() const {
  return periodLengthBytes;
}

void ALSA::setPeriodLengthBytes(snd_pcm_uframes_t periodLengthBytes) {
  ALSA::periodLengthBytes = periodLengthBytes;
}

snd_pcm_uframes_t ALSA::getRequestedBufferSize() const {
  return requestedBufferSizeFrames;
}

void ALSA::setRequestedBufferSize(snd_pcm_uframes_t requestedBufferSize) {
  ALSA::requestedBufferSizeFrames = requestedBufferSize;
}

snd_pcm_t* ALSA::getPcmHandle() const {
  return pcmHandle;
}

void ALSA::setPcmHandle(snd_pcm_t* pcmHandle) {
  ALSA::pcmHandle = pcmHandle;
}

snd_pcm_stream_t ALSA::getPlaybackStream() const {
  return streamDirection;
}

void ALSA::setPlaybackStream(snd_pcm_stream_t playbackStream) {
  ALSA::streamDirection = playbackStream;
}

snd_pcm_hw_params_t* ALSA::getHardwareParams() const {
  return hardwareParams;
}

void ALSA::setHardwareParams(snd_pcm_hw_params_t* hardwareParams) {
  ALSA::hardwareParams = hardwareParams;
}

char* ALSA::getPcmDeviceName() const {
  return pcmDeviceName;
}

void ALSA::setPcmDeviceName(char* pcmDeviceName) {
  ALSA::pcmDeviceName = pcmDeviceName;
}

int ALSA::getDir() const {
  return dir;
}

void ALSA::setDir(int dir) {
  ALSA::dir = dir;
}

unsigned int ALSA::getRequestedSampleRate() const {
  return requestedSamplingRate;
}

void ALSA::setRequestedSampleRate(unsigned int requestedSampleRate) {
  ALSA::requestedSamplingRate = requestedSampleRate;
}

unsigned int ALSA::getBytesPerFrame() const {
  return bytesPerFrame;
}

void ALSA::setBytesPerFrame(unsigned int bytesPerFrame) {
  bytesPerFrame = bytesPerFrame;
}

unsigned long ALSA::getFramesPerPeriod() const {
  return framesPerPeriod;
}

void ALSA::setFramesPerPeriod(unsigned long framesPerPeriod) {
  ALSA::framesPerPeriod = framesPerPeriod;
}

unsigned int ALSA::getNPeriods() const {
  return nPeriods;
}

void ALSA::setNPeriods(unsigned int nPeriods) {
  nPeriods = nPeriods;
}

