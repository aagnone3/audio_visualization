CC=g++
CFLAGS=-g -c -Wall -std=c++14
LDFLAGS=-framework OpenGL -framework GLUT
SOURCES=src/main.cpp src/SpectrogramVisualizer.cpp src/PortAudio.cpp src/shared.cpp src/Log.cpp src/Display.cpp src/AudioInput.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=opengl_spectrogram

# portaudio
CFLAGS+=-I/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/include
LDFLAGS+=-L/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/lib -lportaudio

# fftw
CFLAGS+=-I/Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2/include
LDFLAGS+=-L/Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2/lib -lfftw3f

# custom vars
SRC_DIR=src

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f src/*.o $(EXECUTABLE)

