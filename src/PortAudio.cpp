//
// Created by aagnone3 on 8/17/16.
//

#include "PortAudio.hpp"

PortAudio::PortAudio()
  : AudioInput()
{
    samplingRate = 44100;
    samplingPeriod = 1.0f/samplingRate;
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

PortAudio::~PortAudio() {
    delete stream;
}

int PortAudio::audioIn(const void* inputBuffer, void* outputBuffer, unsigned long numSamples,
                       const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
                       void* customData)
{
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
//        Log::getInstance()->logger() << in[numSamples - 1] << std::endl;
        for (int i = 0; i < numSamples; ++i)
        {
            index = mod(instance->bufferIndex + i, instance->bufferSizeSamples);
            instance->audioBuffer[index] = in[i]; /* left channel */
            //if (NUM_CHANNELS == 2) audioBuffer[i] = *in++; /* right channel */
        }
    }
    instance->bufferIndex = mod(instance->bufferIndex + numSamples, instance->bufferSizeSamples);
    computeSpectrogramSlice(instance);
    
//    Log::getInstance()->logger() << "Buffer Index: " << instance->bufferIndex << std::endl;
//    Log::getInstance()->logger() << "# Samples: " << numSamples << ", Size: " << instance->bufferSizeSamples << std::endl;
    
    return paContinue;
}

void PortAudio::quitNow()
{
    Log::getInstance()->logger() << "Hay2" << std::endl;
    /* raise the quit flag to signal to the audio capture thread to stop polling for audio */
    quit = true;

    /* close the stream */
    PaError err;
    err = Pa_CloseStream(stream);
    Log::getInstance()->logger() << "Closing stream." << std::endl;
    // TODO this is throwing Invalid stream pointer?
//    if (err != paNoError) {
//        Log::getInstance()->logger() << "PortAudio stream closing error: " << Pa_GetErrorText(err) << std::endl;
//    }
    
    /* terminate portaudio */
    err = Pa_Terminate();
    Log::getInstance()->logger() << "Terminating PortAudio." << std::endl;
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
    }
    
    /* get number of devices available */
    int numDevices = Pa_GetDeviceCount();
    Log::getInstance()->logger() << "Number of audio devices discovered: " << numDevices << std::endl;


    /* try to open the default audio stream */
    err = Pa_OpenDefaultStream(&stream,
                               2, /* stereo input */
                               0, /* no output channels (otherwise 1 or 2) */
                               paFloat32,
                               samplingRate,
                               paFramesPerBufferUnspecified,
                               audioIn,
                               (void*)this);
    /* if opening the default stream fails, give the user device info */
    if (err != paNoError) {
        Log::getInstance()->logger() << "PortAudio error opening default stream: " << Pa_GetErrorText(err) << std::endl;

        const PaDeviceInfo *deviceInfo;
        for (int i = 0; i < numDevices; ++i) {
            deviceInfo = Pa_GetDeviceInfo(i);
            Log::getInstance()->logger() << "Device: " << deviceInfo->name
                                         << std::endl << "Max Input: " << deviceInfo->maxInputChannels
                                         << std::endl << "Max Output: " << deviceInfo->maxOutputChannels << std::endl;

        }

        return 1;
    }

    /* start the audio stream */
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        Log::getInstance()->logger() << "PortAudio starting stream: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }
    
    Log::getInstance()->logger() << "Successfully starting sampling audio." << std::endl;
    return 0;
}

