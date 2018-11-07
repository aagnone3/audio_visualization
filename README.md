# Audio Visualization with OpenGL in C++

This is a rework of a real-time spectrogram based upon OpenGL, GLUT, and PortAudio.
Original project: [Real-time OpenGL spectrogram by Alex Barnett](https://math.dartmouth.edu/~ahb/software/#glspect)

# Dependencies

- \*nix operating system with
- libraries
  - portaudio
  - fftw2
  - opengl
  - glut

# Building

```bash
make build
build/opengl_spectrogram
```

# Installing
```bash
make install
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
