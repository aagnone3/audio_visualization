#include <iostream>
#include <portaudio.h>


const unsigned int GOOD = 0;
const unsigned int ERROR = 1;


void _showDeviceInfo(int i)
{
    const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
    std::cout << "[" << i << "]: " << deviceInfo->name << std::endl << "\tMax Input: " << deviceInfo->maxInputChannels
        << std::endl << "\tMax Output: " << deviceInfo->maxOutputChannels << std::endl;
}

int showDeviceInfo() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cout << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
        return ERROR;
    }

    const PaDeviceInfo *deviceInfo;

    /* get number of devices available */
    int numDevices = Pa_GetDeviceCount();
    if ( numDevices < 0 )
    {
        std::cerr << "ERROR: Pa_GetDeviceCount returned " << numDevices << std::endl;
        return ERROR;
    }
    std::cout << "Number of audio devices discovered: " << numDevices << std::endl;

    /* show device information for all devices, including the designated default */
    std::cout << "Default device" << std::endl
              << "--------------" << std::endl;
    _showDeviceInfo(Pa_GetDefaultInputDevice());

    std::cout << std::endl
              << "All devices" << std::endl
              << "-----------" << std::endl;
    for (int i = 0; i < numDevices; ++i) {
        _showDeviceInfo(i);
    }

    return GOOD;
}

int main(int argc, char** argv)
{
    return showDeviceInfo();
}
