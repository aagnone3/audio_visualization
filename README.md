# Audio Visualization with OpenGL in C++

This is a rework of a real-time spectrogram based upon OpenGL, GLUT, and PortAudio.
Original project: [Real-time OpenGL spectrogram by Alex Barnett](https://math.dartmouth.edu/~ahb/software/#glspect)

# Dependencies

- \*nix operating system with
- libraries
  - portaudio
  - fftw
  - opengl etc

```bash
sudo apt install -y
    cmake \
    libasound2-dev \
    portaudio19-dev \
    libfftw3-dev \
    libglu1-mesa-dev \
    freeglut3-dev \
    mesa-common-dev
```

# Building

```bash
make build

# run with the following
build/opengl_spectrogram
```

# Installing
```bash
make install

# run with the following
opengl_spectrogram
```

# Helpers
```bash
# show info regarding audio devices recognized by the OS
device_info
```

# Screenshot

Here's a quick screenshot of [this song](https://www.youtube.com/watch?v=n70c3Dzw-ZM) around 1:45 on. Can you line it up?

![Visualization Screenshot](https://raw.githubusercontent.com/aagnone3/audio_visualization/master/res/img/screenshot_armin.png)

# Documentation

See doc/ for Doxygen documentation of the source code.

## Contributors

Currently myself and Alex Barnett. We are happy to enhance this project with others, don't hesitate to reach out!
A special thanks goes out to Alex Barnett for the initial version of this project.
