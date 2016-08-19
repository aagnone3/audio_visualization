/**
 *  Generic interface to the Advanced Linux Sound Architecture (ALSA) software framework.
 *  Based off of the ALSA tutorial at http://www.suse.de/~mana/alsa090_howto.html
 *  @author Anthony Agnone, Aug 2016
 */

#ifndef OPENGL_SPECTROGRAM_ALSAINTERFACE_HPP
#define OPENGL_SPECTROGRAM_ALSAINTERFACE_HPP

#include <iostream>
#include <alsa/asoundlib.h>
#include "AudioInput.hpp"
#include "Log.hpp"

class ALSA : public AudioInput {

public:
  /**
   * Callable used by a thread to continuously populate audioBuffer from the audio stream.
   * @param alsaInputInstance instance of ALSAInput to share audio buffer data with.
   */
  static void audioCapture(void* alsaInputInstance);

  /**
   * Constructor
   */
  ALSA();

  /**
   * Closes the audio capture stream and de-allocates any dynamic memory.
   */
  ~ALSA();

  /**
   * Sets up the sound card for recording.
   * @return indication of setup success or failure.
   *    -1 -> failed setup.
   *    0 -> successfull setup.
   */
  int initDevice();

  /**
   * Defines how the instance stops the audio stream.
   */
  virtual void quitNow();

private:
  /**
   * Length of each ALSA interrupt period, in bytes.
   */
  snd_pcm_uframes_t periodLengthBytes;

  /**
   * Frame size of the buffer requested from ALSA during initialization.
   */
  snd_pcm_uframes_t requestedBufferSizeFrames;

  /**
   * Sampling rate requested from ALSA during initialization.
   */
  unsigned int requestedSamplingRate;

  /**
   * Struct handle on the PCM device on the computer.
   */
  snd_pcm_t* pcmHandle;

  /**
   * Enum to represent the current direction of sound streaming.
   */
  snd_pcm_stream_t streamDirection;

  /**
   * Structure containing information about
    the hardware. This can also be used to specify the
    configuration to be used for the PCM stream.
   */
  snd_pcm_hw_params_t* hardwareParams;

  /**
   * Unique identifier for the PCM device, e.g. plughw:0,0.
    The first number is the number of the sound card.
    The second number is the number of the device.
   */
  char* pcmDeviceName;

  /**
   * Comparison of the requested sampling rate to the sampling rate finalized by ALSA initialization.
   *    samplingRate == requestedSampleRate --> dir = 0
   *    samplingRate > requestedSampleRate  --> dir = 1
   *    samplingRate < requestedSampleRate  --> dir = -1
   */
  int dir;

  /**
   * Number of bytes in each audio frame.
   */
  unsigned int bytesPerFrame;

  /**
   * Number of audio frames in each ALSA interrupt period.
   */
  unsigned long framesPerPeriod;

  /**
   * Number of ALSA interrupt periods to use.
   */
  unsigned int nPeriods;

  /**
   * Shared bit-space used to convert from signed to unsigned.
   */
  union byte {
    unsigned char unsignedVal;
    char signedVal;
  };

/* getters and setters */
  snd_pcm_uframes_t getPeriodLengthBytes() const;

  void setPeriodLengthBytes(snd_pcm_uframes_t periodLengthBytes);

  snd_pcm_uframes_t getRequestedBufferSize() const;

  void setRequestedBufferSize(snd_pcm_uframes_t requestedBufferSize);

  snd_pcm_t* getPcmHandle() const;

  void setPcmHandle(snd_pcm_t* pcmHandle);

  snd_pcm_stream_t getPlaybackStream() const;

  void setPlaybackStream(snd_pcm_stream_t playbackStream);

  snd_pcm_hw_params_t* getHardwareParams() const;

  void setHardwareParams(snd_pcm_hw_params_t* hardwareParams);

  char* getPcmDeviceName() const;

  void setPcmDeviceName(char* pcmDeviceName);

  int getDir() const;

  void setDir(int dir);

  unsigned int getRequestedSampleRate() const;

  void setRequestedSampleRate(unsigned int requestedSampleRate);

  unsigned int getBytesPerFrame() const;

  void setBytesPerFrame(unsigned int bytesPerFrame);

  unsigned long getFramesPerPeriod() const;

  void setFramesPerPeriod(unsigned long framesPerPeriod);

  unsigned int getNPeriods() const;

  void setNPeriods(unsigned int nPeriods);
};
#endif /* OPENGL_SPECTROGRAM_ALSAINTERFACE_HPP */
