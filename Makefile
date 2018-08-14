CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
SRC_DIR=src
SOURCES=$(SRC_DIR)/*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run

## portaudio
PA_DIR=/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0
CFLAGS+=-I$(PA_DIR)/include
LDFLAGS+=-L$(PA_DIR)/lib -lportaudio

## fftw
FFTW_DIR=/Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2
CFLAGS+=-I$(FFTW_DIR)/include
LDFLAGS+=-L$(FFTW_DIR)/lib -lfftw3f

## yaml
YAML_DIR=/Users/aagnone/github/brew/brew/Cellar/yaml-cpp/0.6.2
CFLAGS+=-I$(YAML_DIR)/include
LDFLAGS+=-L$(YAML_DIR)/lib -lyaml-cpp

# build executable
build: $(OBJECTS) $(EXECUTABLE)

# build executable and run
exec: $(SOURCES) $(EXECUTABLE)
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE)
