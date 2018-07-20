cmake_minimum_required(VERSION 3.5)
project(opengl_spectrogram)
set(SOURCE_FILES
        src/main.cpp
        src/AudioInput.hpp
        src/AudioInput.cpp
        src/SpectrogramVisualizer.hpp
        src/SpectrogramVisualizer.cpp
        src/Log.hpp
        src/Log.cpp
        src/Display.hpp
        src/Display.cpp
        src/GraphicsItem.hpp
        src/PortAudio.hpp
        src/PortAudio.cpp
        src/shared.hpp
        src/shared.cpp)
add_executable(opengl_spectrogram ${SOURCE_FILES})


# ================= Dependencies =================

set(BREW_DIR /Users/aagnone/github/brew/brew/Cellar)

# OpenGL/GLUT
find_package(OpenGL REQUIRED)
#find_package(GL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(opengl_spectrogram ${OPENGL_LIBRARIES})
target_link_libraries(opengl_spectrogram ${GLUT_LIBRARIES})

## FFTW3
#find_library(FFTW_L
#        NAMES fftw3.a
#        PATH_PREFIXES "${BREW_DIR}"
#        )
#message(STATUS ${FFTW_L})
#target_link_libraries(opengl_spectrogram "${FFTW_L}")
#
#find_path(FFTW_I
#        NAMES fftw3.h
#        PATHS /Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2/include
#        )
#message(STATUS "${FFTW_I}")
#target_include_directories(opengl_spectrogram "${FFTW_I}")
#target_include_directories(opengl_spectrogram /Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2/include)

# PortAudio
#find_library(PORTAUDIO_L
#        NAMES portaudio.a
#        PATHS /Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/lib
##        PATH_PREFIXES "${BREW_DIR}"
#        )
#find_path(PORTAUDIO_I
#        NAMES portaudio.h
#        PATHS /Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/include
#        )
#message(STATUS ${PORTAUDIO_L})
#message(STATUS "${PORTAUDIO_I}")
#target_link_libraries(opengl_spectrogram "${PORTAUDIO_L}")
#target_include_directories(opengl_spectrogram "${PORTAUDIO_I}")
#include_directories("/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/include")
#link_directories("/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/lib")

set(PORTAUDIO "/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0")
set(FFTW "/Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2")

include_directories(
        ${PORTAUDIO}/include
        ${FFTW}/include
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${FFTW}/lib -lfftw3f")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${PORTAUDIO}/lib -lportaudio")
