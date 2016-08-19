cmake_minimum_required(VERSION 3.5)
project(opengl_spectrogram)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglut")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lasound -lfftw3f -lm -pthread")

set(SOURCE_FILES main.cpp AudioInput.cpp AudioInput.hpp SpectrogramVisualizer.hpp SpectrogramVisualizer.cpp Log.cpp Log.hpp Display.cpp Display.hpp GraphicsItem.hpp ALSA.cpp ALSA.hpp shared.hpp shared.cpp)
add_executable(opengl_spectrogram ${SOURCE_FILES})
