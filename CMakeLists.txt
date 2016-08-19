cmake_minimum_required(VERSION 3.5)
project(opengl_spectrogram)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglut")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lasound -lfftw3f -lm -pthread")

set(SOURCE_FILES src/main.cpp src/AudioInput.cpp src/AudioInput.hpp src/SpectrogramVisualizer.hpp src/SpectrogramVisualizer.cpp src/Log.cpp src/Log.hpp src/Display.cpp src/Display.hpp src/GraphicsItem.hpp src/ALSA.cpp src/ALSA.hpp src/shared.hpp src/shared.cpp)
add_executable(opengl_spectrogram ${SOURCE_FILES})
