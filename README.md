# Audio Visualization with OpenGL in C++

This is a rework-in-progress of a real-time spectrogram based upon OpenGL, GLUT, and ALSA. Here's a few examples of what you can tweak with it:
- Color range
- Contrast
- Sampling rate
- Frequency range

Original project: [Real-time OpenGL spectrogram by Alex Barnett](https://math.dartmouth.edu/~ahb/software/#glspect)

# Dependencies

- linux operating system with ALSA
- linus programs
  - cmath
  - fftw
  - fftw-devel
  - alsa-lib
  - alsa-lib-devel

# Installation

Compile via [CMake](https://cmake.org/cmake-tutorial/) or by a simple re-use of the provided [CLion](https://www.jetbrains.com/clion/) .idea/ directory.

# Snapshot

![Visualization Screenshot](https://raw.githubusercontent.com/aagnone3/audio_visualization/master/res/img/screenshot.png)

# Documentation

See doc/ for Doxygen documentation of the source code.

## Contributors

Currently me, myself, and I. I'm happy to enhance this project with others, don't hesitate to reach out!
A special thanks goes out to Alex Barnett for the initial versions of this project.

## License

This software is released with the Apache License. Download it, use it, change it, share it. Just keep the license!