#include <iostream>
#include <cstring>
#include <portaudio.h>


int audioIn(const void* inputBuffer, void* outputBuffer, unsigned long numSamples,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
        void* customData)
{
    std::cout << "# Samples: " << numSamples << std::endl;
    return paContinue;
}

int get_device_id(const char *fn)
{
    //YAML::Node config = YAML::LoadFile(fn);
    //return config["device_id"].as<int>();
    return 2;
} 

void showDeviceInfo() {
    const PaDeviceInfo *deviceInfo;

    /* get number of devices available */
    int numDevices = Pa_GetDeviceCount();
    std::cout << "Number of audio devices discovered: " << numDevices << std::endl;

    for (int i = 0; i < numDevices; ++i) {
        deviceInfo = Pa_GetDeviceInfo(i);
        std::cout << "[" << i << "]: " << deviceInfo->name << std::endl << "\tMax Input: " << deviceInfo->maxInputChannels
            << std::endl << "\tMax Output: " << deviceInfo->maxOutputChannels << std::endl;

    }
}


int stream_device_id(int device_id) {
    /* initialize and zero out the input parameters struct */
    PaStreamParameters inputParams;
    const PaDeviceInfo *deviceInfo;
    std::memset(&inputParams, 0, sizeof(inputParams));

    /* populate the input params */
    deviceInfo = Pa_GetDeviceInfo(device_id);
    inputParams.channelCount = deviceInfo->maxInputChannels;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = deviceInfo->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    PaStream *stream = nullptr;
    PaError err = Pa_OpenStream(
            &stream,
            &inputParams,
            NULL,
            44100,
            paFramesPerBufferUnspecified,
            paNoFlag,
            audioIn,
            nullptr
    );

    return err == paNoError ? 0 : 1;
}

bool test_device(int device_id) {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cout << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    if (device_id == -1) {
        /* try to open the default audio stream */
        PaStream *stream = nullptr;
        err = Pa_OpenDefaultStream(&stream,
                                   2, /* stereo input */
                                   0, /* no output channels (otherwise 1 or 2) */
                                   paFloat32,
                                   44100,
                                   paFramesPerBufferUnspecified,
                                   audioIn,
                                   nullptr);

        /* if opening the default stream fails, give the user device info */
        if (err != paNoError) {
            showDeviceInfo();
            return 1;
        }

        /* start the audio stream */
        err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cout << "PortAudio starting stream: " << Pa_GetErrorText(err) << std::endl;
            return 1;
        }
        
        std::cout << "Successfully starting sampling audio." << std::endl;
        return 0;
    } else {
        /* open the device with the requested id */
        return stream_device_id(device_id);
    }
}


int main(int argc, char** argv)
{
    int device_id = get_device_id("cfg.yaml");
    std::cout << "Device id from configuration: " << device_id << std::endl;
    return test_device(device_id);    
}
