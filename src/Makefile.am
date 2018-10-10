AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

AM_CXXFLAGS = -fpic -Wall -Wextra --std=c++14 -g -I/Users/aagnone/github/brew/brew/Cellar/yaml-cpp/0.6.2/include -I/Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2/include -I/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/include
AM_LDFLAGS = -fpic -Wall -Wextra --std=c++14 -g -framework OpenGL -framework GLUT -L/Users/aagnone/github/brew/brew/Cellar/yaml-cpp/0.6.2/lib -L/Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2/lib -L/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/lib -lportaudio -lfftw3f -lyaml-cpp

bin_PROGRAMS = audio_visualization
audio_visualization_SOURCES = main.cpp \
							  AudioInput.cpp \
                              AudioInput.hpp \
                              Display.cpp \
                              Display.hpp \
                              GraphicsItem.hpp \
                              Log.cpp \
                              Log.hpp \
                              PortAudio.cpp \
                              PortAudio.hpp \
                              SpectrogramVisualizer.cpp \
                              SpectrogramVisualizer.hpp \
                              shared.cpp \
                              shared.hpp
