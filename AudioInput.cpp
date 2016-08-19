#include "AudioInput.hpp"
#include "ALSA.hpp"
#include <boost/log/trivial.hpp>

/* static member declarations and initializations */
const unsigned int AudioInput::VERBOSITY = 2;
const unsigned int AudioInput::N_FREQUENCIES = 560;
const unsigned int AudioInput::N_TIME_WINDOWS = 940;

AudioInput::AudioInput()
{
  quit = false;
  pause = false;

  // TODO change how class receives value for windowSizeExponent
  unsigned int twowinsize = 13; // 8192 samples (around 0.19 sec). Remains fixed
  fftLength = (unsigned int) 1 << twowinsize;
  windowedAudioFrame = new float[fftLength];

  windowingFunction = new float[fftLength];
  setupWindowFunc(windowingFunction, fftLength, 2);
  /* set up in-place single-precision real-to-half-complex FFT */
  fftPlan = fftwf_plan_r2r_1d(fftLength, windowedAudioFrame, windowedAudioFrame, FFTW_R2HC, FFTW_MEASURE);
  spectrogramSlice = new float[N_FREQUENCIES];
  spectrogramSize = N_FREQUENCIES*N_TIME_WINDOWS;
}

AudioInput::~AudioInput() {}

void AudioInput::setupWindowFunc(float* window, int windowSize, unsigned int windowType)
{
  float W;
  int i;

  switch (windowType) {
  case 0:
    /* no window (crappy frequency spillover) */
    for (i = 0; i<windowSize; ++i)
      window[i] = 1.0F;
    break;
  case 1:
    /* Hann window (C^1 cont, so third-order tails) */
    W = windowSize/2.0F;
    for (i = 0; i<windowSize; ++i)
      window[i] = (float) (1.0f+cos(M_PI*(i-W)/W))/2;
    break;
  case 2:
    /* truncated Gaussian window (Gaussian tails + exp small error) */
    /* width: keep small truncation but wide to not waste FFT */
    W = windowSize/5.0F;
    for (i = 0; i<windowSize; ++i) {
      window[i] = (float) exp(-(i-windowSize/2)*(i-windowSize/2)/(2*W*W));
    }
    std::cout << W << std::endl;
    break;
  default:
    fprintf(stderr, "unknown windowType!\n");
  }
}

void AudioInput::computeSpectrogramSlice(AudioInput* audioInput)
{
  int N = audioInput->fftLength;             // transform length
  int nf = audioInput->N_FREQUENCIES;              // # freqs to fill in powerspec

  /* copy last N samples & multiply by the window */
  for (int i = 0; i<N; ++i) {
    audioInput->windowedAudioFrame[i] = audioInput->windowingFunction[i]*audioInput->audioBuffer[mod(audioInput->bufferIndex-N+i, audioInput->bufferSizeSamples)];
  }
  /* execute the planned FFT */
  fftwf_execute(audioInput->fftPlan);

  if (nf>N/2) {
    fprintf(stderr, "window too short cf n_f!\n");
    return;
  }
  /* zero-frequency has no imaginary part */
  audioInput->spectrogramSlice[0] = audioInput->windowedAudioFrame[0]*audioInput->windowedAudioFrame[0];
  /* compute power spectrum from hc dft */
  for (int i = 1; i<nf; ++i) {
    audioInput->spectrogramSlice[i] =
        audioInput->windowedAudioFrame[i]*audioInput->windowedAudioFrame[i]+audioInput->windowedAudioFrame[N-i]*audioInput->windowedAudioFrame[N-i];
  }
}

const unsigned int AudioInput::getVERBOSITY() {
  return VERBOSITY;
}

const unsigned int AudioInput::getN_FREQUENCIES() {
  return N_FREQUENCIES;
}

const unsigned int AudioInput::getN_TIME_WINDOWS() {
  return N_TIME_WINDOWS;
}

float* AudioInput::getAudioBuffer() const {
  return audioBuffer;
}

void AudioInput::setAudioBuffer(float* audioBuffer) {
  AudioInput::audioBuffer = audioBuffer;
}

float* AudioInput::getSpectrogramSlice() const {
  return spectrogramSlice;
}

void AudioInput::setSpectrogramSlice(float* spectrogramSlice) {
  AudioInput::spectrogramSlice = spectrogramSlice;
}

float* AudioInput::getWindowingFunction() const {
  return windowingFunction;
}

void AudioInput::setWindowingFunction(float* windowingFunction) {
  AudioInput::windowingFunction = windowingFunction;
}

unsigned int AudioInput::getSpectrogramSize() const {
  return spectrogramSize;
}

void AudioInput::setSpectrogramSize(unsigned int spectrogramSize) {
  AudioInput::spectrogramSize = spectrogramSize;
}

unsigned int AudioInput::getFftLength() const {
  return fftLength;
}

void AudioInput::setFftLength(unsigned int fftLength) {
  AudioInput::fftLength = fftLength;
}

float AudioInput::getBufferMemorySeconds() const {
  return bufferMemorySeconds;
}

void AudioInput::setBufferMemorySeconds(float bufferMemorySeconds) {
  AudioInput::bufferMemorySeconds = bufferMemorySeconds;
}

unsigned int AudioInput::getNChannels() const {
  return nChannels;
}

void AudioInput::setNChannels(unsigned int nChannels) {
  AudioInput::nChannels = nChannels;
}

float AudioInput::getSamplingPeriod() const {
  return samplingPeriod;
}

void AudioInput::setSamplingPeriod(float samplingPeriod) {
  AudioInput::samplingPeriod = samplingPeriod;
}

bool AudioInput::isQuit() const {
  return quit;
}

void AudioInput::setQuit(bool quit) {
  AudioInput::quit = quit;
}

bool AudioInput::isPause() const {
  return pause;
}

void AudioInput::setPause(bool pause) {
  AudioInput::pause = pause;
}

unsigned int AudioInput::getSamplingRate() const {
  return samplingRate;
}

void AudioInput::setSamplingRate(unsigned int samplingRate) {
  AudioInput::samplingRate = samplingRate;
}

char* AudioInput::getAudioBufferChunk() const {
  return audioBufferChunk;
}

void AudioInput::setAudioBufferChunk(char* audioBufferChunk) {
  AudioInput::audioBufferChunk = audioBufferChunk;
}

int AudioInput::getBufferIndex() const {
  return bufferIndex;
}

void AudioInput::setBufferIndex(int bufferIndex) {
  AudioInput::bufferIndex = bufferIndex;
}

float* AudioInput::getWindowedAudioFrame() const {
  return windowedAudioFrame;
}

void AudioInput::setWindowedAudioFrame(float* windowedAudioFrame) {
  AudioInput::windowedAudioFrame = windowedAudioFrame;
}

unsigned long AudioInput::getBufferSizeFrames() const {
  return bufferSizeFrames;
}

void AudioInput::setBufferSizeFrames(unsigned long bufferSizeFrames) {
  AudioInput::bufferSizeFrames = bufferSizeFrames;
}

int AudioInput::getBufferSizeSamples() const {
  return bufferSizeSamples;
}

void AudioInput::setBufferSizeSamples(int bufferSizeSamples) {
  AudioInput::bufferSizeSamples = bufferSizeSamples;
}
