//
// Created by aagnone3 on 8/17/16.
//

#include "PortAudio.hpp"
#include <cstring>

PortAudio::PortAudio(int requestedInputDeviceId)
  : AudioInput()
{
    this->requestedInputDeviceId = requestedInputDeviceId;
    samplingRate = 44100;
    samplingPeriod = 1.0f/samplingRate;
    //stream = nullptr;
    bufferMemorySeconds = 5;
    bufferSizeSamples = bufferMemorySeconds * samplingRate;
    
    Log::getInstance()->logger() << "Buffer Size: " << bufferSizeSamples << " samples." << std::endl;
    
    /* initialize the audio buffer */
    audioBuffer = new float[bufferSizeSamples];
    for (int i = 0; i < bufferSizeSamples; ++i)
    {
        audioBuffer[i] = 0.0f;
    }
}

PortAudio::PortAudio(const PortAudio& other) {
    this->stream = other.stream;
    this->requestedInputDeviceId = other.requestedInputDeviceId;
}

PortAudio& PortAudio::operator=(const PortAudio& other)
{
    this->stream = other.stream;
    this->requestedInputDeviceId = other.requestedInputDeviceId;

    return *this;
}

PortAudio::~PortAudio() {
    //delete stream;
}

int PortAudio::audioIn(const void* inputBuffer, void* outputBuffer, unsigned long numSamples,
                       const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
                       void* customData)
{
    //Log::getInstance()->logger() << "audioIn()" << std::endl;
    PortAudio *instance = (PortAudio*)customData;
    const SAMPLE *in = (SAMPLE*)inputBuffer;
    int index;
    
    /* prevent unused variable warnings */
    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;
    
    if (inputBuffer == NULL)
    {
        /* silence */
        for (int i = 0; i < numSamples; ++i)
        {
            index = mod(instance->bufferIndex + i, instance->bufferSizeSamples);
            instance->audioBuffer[index] = SAMPLE_SILENCE; /* left channel */
            //if (NUM_CHANNELS == 2) audioBuffer[i] = SAMPLE_SILENCE; /* right channel */
        }
    }
    else
    {
        /* non-silence */
        //Log::getInstance()->logger() << in[numSamples - 1] << std::endl;
        for (int i = 0; i < numSamples; ++i)
        {
            index = mod(instance->bufferIndex + i, instance->bufferSizeSamples);
            instance->audioBuffer[index] = in[i]; /* left channel */
            //if (NUM_CHANNELS == 2) audioBuffer[i] = *in++; /* right channel */
        }
    }
    instance->bufferIndex = mod(instance->bufferIndex + numSamples, instance->bufferSizeSamples);
    computeSpectrogramSlice(instance);
    
    //Log::getInstance()->logger() << "Buffer Index: " << instance->bufferIndex << std::endl;
    //Log::getInstance()->logger() << "# Samples: " << numSamples << ", Size: " << instance->bufferSizeSamples << std::endl;
    
    return instance->quit ? paComplete : paContinue;
}

void PortAudio::quitNow()
{
    Log::getInstance()->logger() << "Quitting." << std::endl;
    /* raise the quit flag to signal to the audio capture thread to stop polling for audio */
    quit = true;

    /* close the stream */
    Log::getInstance()->logger() << "Closing stream." << std::endl;
    PaError err;
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        Log::getInstance()->logger() << "PortAudio stream close error: " << Pa_GetErrorText(err) << std::endl;
    }
    
    /* terminate portaudio */
    Log::getInstance()->logger() << "Terminating PortAudio." << std::endl;
    err = Pa_Terminate();
    if (err != paNoError) {
        Log::getInstance()->logger() << "PortAudio termination error: " << Pa_GetErrorText(err) << std::endl;
    }
}

int PortAudio::startCapture()
{
    PaError err;
    
    err = Pa_Initialize();
    if (err != paNoError) {
        Log::getInstance()->logger() << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    Log::getInstance()->logger() << "Opening input stream for device #" << requestedInputDeviceId << std::endl;

    /* initialize and populate desired input stream parameters */
    PaStreamParameters inputParams;
    const PaDeviceInfo *deviceInfo;
    memset(&inputParams, 0, sizeof(inputParams));
    deviceInfo = Pa_GetDeviceInfo(requestedInputDeviceId);
    inputParams.channelCount = 1;//deviceInfo->maxInputChannels;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = deviceInfo->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    /* try to open the requested stream */
    err = Pa_OpenStream(
            &stream,
            &inputParams,
            NULL,
            44100,
            paFramesPerBufferUnspecified,
            paNoFlag,
            audioIn,
            (void*)this
    );

    /* if an error occurs, try to open the default stream instead */
    if (err != paNoError) {
        Log::getInstance()->logger() << std::endl << "PortAudio error opening requested stream: " << Pa_GetErrorText(err) << "."
            << std::endl << std::endl
            << "Attempting to open the default stream." << std::endl;

        err = Pa_OpenDefaultStream(
                &stream,
                1,  // # input channels
                0,  // # output channels
                paFloat32,
                44100,  // sampling rate
                paFramesPerBufferUnspecified,
                audioIn,
                (void*)this
        );
        if (err != paNoError) {
            Log::getInstance()->logger() << "PortAudio error opening default stream: " << Pa_GetErrorText(err) << "." << std::endl;

            /* TODO if opening the default stream fails, give the user device info */
            return 1;
        }

    }
    
    /* start the audio stream */
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        Log::getInstance()->logger() << "PortAudio error starting stream: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }
    
    Log::getInstance()->logger() << "Successfully starting sampling audio." << std::endl;
    return 0;
}
