/**
 *  Interface to the cross-platform PortAudio audio interface software framework.
 *  @author Anthony Agnone, Aug 2017
 */

#ifndef OPENGL_SPECTROGRAM_PORTAUDIOINTERFACE_HPP
#define OPENGL_SPECTROGRAM_PORTAUDIOINTERFACE_HPP

#include <iostream>
#include <portaudio.h>
#include "AudioInput.hpp"
#include "Log.hpp"

typedef float SAMPLE;
#define SAMPLE_SILENCE (0.0f)


class PortAudio : public AudioInput {

public:
  /**
   * Callable used by a thread to continuously populate audioBuffer from the audio stream.
   */
    static int audioIn(const void* inputBuffer, void* outputBuffer, unsigned long numSamples,
                       const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
                       void* userData);

  /**
   * Constructor
   */
  PortAudio();

  /**
   * Closes the audio capture stream and de-allocates any dynamic memory.
   */
  ~PortAudio();

  /**
   * Begins capturing the audio stream.
   * @return indication of capture success or failure.
   *    0 -> successfull setup.
   *    else -> failed setup.
   */
    int startCapture();

  /**
   * Defines how the instance stops the audio stream.
   */
  virtual void quitNow();

private:
    /**
     * Audio stream pointer.
     */
    PaStream *stream;

};
#endif /* OPENGL_SPECTROGRAM_PORTAUDIOINTERFACE_HPP */
