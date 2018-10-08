/**
 * Generic interface to obtaining audio input.
 * @author Anthony Agnone, Aug 2016
 *
 * TODO generalize to audio input or output.
 * TODO obtain shared verbosity indicator.
 * TODO obtain parameters from command-line.
 */

#ifndef OPENGL_SPECTROGRAM_AUDIOINPUT_H
#define OPENGL_SPECTROGRAM_AUDIOINPUT_H

#include <thread>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <fftw3.h>
#include "Log.hpp"
#include "shared.hpp"

class AudioInput {
public:
  /**
   * Level of verbosity for logging.
   */
  static const unsigned int VERBOSITY;

  /**
   * Number of spectrogram frequencies.
   */
  static const unsigned int N_FREQUENCIES;

  /**
   * Number of time windows. This should be multiple of 4 for glDrawPixels.
   */
  static const unsigned int N_TIME_WINDOWS;

  /**
   * Overloaded constructor to initialize various member parameters.
   */
  AudioInput();

  AudioInput(const AudioInput&);
  AudioInput& operator=(const AudioInput&);

  /**
   * Destructor de-allocates all dynamic memory and joins the audio retrieval thread.
   */
  virtual ~AudioInput() = 0;

  /**
   * Sets up the windowing function to avoid the harmful spectral effects of a rectangular window.
   * Note: windowSize is intentionally auto-casted to a signed integer to avoid inf result on exponential calculation.
   * TODO this does not belong in this class.
   * @param window array to hold the window coefficients.
   * @param windowSize length of the window.
   * @param windowType window type.
   *    0: rectangular window.
   *    1: Hann window.
   *    2: truncated Gaussian window.
   */
  void initializeWindow(float* window, int windowSize, unsigned int windowType);

  /**
   * Obtains a windowed spectrogram of the audio stream.
   * TODO this does not belong in this class.
   * @param audioInput  AudioInput handle which contains the audio data to window.
   */
  static void computeSpectrogramSlice(AudioInput* audioInput);

  /**
   * Defines how the instance stops capturing the audio stream.
   */
  virtual void quitNow() = 0;
    
    /**
     * Defines how the instance starts capturing the audio stream.
     */
    virtual int startCapture() = 0;

protected:
  /**
   * Size of the audio buffer that ALSA reports during device intiialization, in number of frames.
   */
  unsigned long bufferSizeFrames;

  /**
   * Size of the audio buffer that ALSA reports during device intiialization, in number of samples.
   */
  int bufferSizeSamples;

  /**
   * Float representation of raw data from the audio buffer for a PCM read of 1 period.
   */
  float* audioBuffer;

  /**
   * Index into audioBuffer;
   */
  int bufferIndex;

  /**
   * Spectrogram data computed from the raw audio buffer data in audioBuffer.
   */
  float* spectrogramSlice;

  /**
   * Window coefficients to be applied to each audio buffer frame.
   */
  float* windowingFunction;

    /**
     * TODO
     */
    float* fftFrame;

  /**
   * Resulting frame of audio data after applying the window coefficients in windowingFunction.
   */
  float* windowedAudioFrame;

  /**
   * Number of samples in each spectrogram frame.
   */
  unsigned int spectrogramSize;

  /**
   * Number of samples in each FFT.
   */
  unsigned int fftLength;

  /**
   * Number of seconds accounted for by the audio buffer.
   */
  float bufferMemorySeconds;

  /**
   * Number of channels in the audio stream.
   * 1 -> mono
   * 2 -> stereo
   */
  unsigned int nChannels;

  /**
   * Frequency of sampling of the audio stream, in units of samples/second.
   */
  unsigned int samplingRate;

  /**
   * Inverse of samplingRate.
   */
  float samplingPeriod;

  /**
   * Flag to to stop sampling the audio stream.
   */
  bool quit;

  /**
   * Flag to pause sampling the audio stream.
   */
  bool pause;

  /**
   * Plan of FFT execution.
   */
  fftwf_plan fftPlan;

  /**
   * Thread used to asynchronously capture audio data into audioBuffer.
   */
  //std::unique_ptr<std::thread> captureThread;

/* accessors (TODO are these necessary?) */
public:
  static const unsigned int getVERBOSITY();

  static const unsigned int getN_FREQUENCIES();

  static const unsigned int getN_TIME_WINDOWS();

  unsigned long getBufferSizeFrames() const;

  void setBufferSizeFrames(unsigned long bufferSizeFrames);

  int getBufferSizeSamples() const;

  void setBufferSizeSamples(int bufferSizeSamples);

  float* getAudioBuffer() const;

  void setAudioBuffer(float* audioBuffer);

  float* getSpectrogramSlice() const;

  void setSpectrogramSlice(float* spectrogramSlice);

  float* getWindowingFunction() const;

  void setWindowingFunction(float* windowingFunction);

  unsigned int getSpectrogramSize() const;

  void setSpectrogramSize(unsigned int spectrogramSize);

  unsigned int getFftLength() const;

  void setFftLength(unsigned int fftLength);

  float getBufferMemorySeconds() const;

  void setBufferMemorySeconds(float bufferMemorySeconds);

  unsigned int getNChannels() const;

  void setNChannels(unsigned int nChannels);

  float getSamplingPeriod() const;

  void setSamplingPeriod(float samplingPeriod);

  bool isQuit() const;

  void setQuit(bool quit);

  bool isPause() const;

  void setPause(bool pause);

  unsigned int getSamplingRate() const;

  void setSamplingRate(unsigned int samplingRate);

  int getBufferIndex() const;

  void setBufferIndex(int bufferIndex);

  float* getWindowedAudioFrame() const;

  void setWindowedAudioFrame(float* windowedAudioFrame);
};

#endif /* OPENGL_SPECTROGRAM_AUDIOINPUT_H */
