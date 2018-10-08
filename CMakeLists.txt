cmake_minimum_required(VERSION 3.5)
project(opengl_spectrogram)
set(DIR_SRC src)
set(DIR_UTIL ${DIR_SRC}/util)
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

# library path variables
set(PORTAUDIO "/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0")
set(FFTW "/Users/aagnone/github/brew/brew/Cellar/fftw/3.3.6-pl2")
set(YAML "/Users/aagnone/github/brew/brew/Cellar/yaml-cpp/0.6.2")

# ================= Executables =================
add_executable(opengl_spectrogram ${SOURCE_FILES})
add_executable(get_devices_info ${DIR_UTIL}/showAllDeviceInfo.cpp)
add_executable(test_device ${DIR_UTIL}/testInput.cpp)

# ================= Dependencies =================
# OpenGL/GLUT
find_package(OpenGL REQUIRED)
#find_package(GL REQUIRED)  # linux
find_package(GLUT REQUIRED)  # osx

# include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lglut")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lGL")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${FFTW}/lib -lfftw3f")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${PORTAUDIO}/lib -lportaudio")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${YAML}/lib -lyaml-cpp")
target_link_libraries(opengl_spectrogram
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

include_directories(
    ${PORTAUDIO}/include
    ${FFTW}/include
    ${YAML}/include
)

#set(BREW_DIR /Users/aagnone/github/brew/brew/Cellar)
# FFTW3
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
#
## PortAudio
##find_library(PORTAUDIO_L
##        NAMES portaudio.a
##        PATHS /Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/lib
###        PATH_PREFIXES "${BREW_DIR}"
##        )
##find_path(PORTAUDIO_I
##        NAMES portaudio.h
##        PATHS /Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/include
##        )
##message(STATUS ${PORTAUDIO_L})
##message(STATUS "${PORTAUDIO_I}")
##target_link_libraries(opengl_spectrogram "${PORTAUDIO_L}")
##target_include_directories(opengl_spectrogram "${PORTAUDIO_I}")
##include_directories("/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/include")
##link_directories("/Users/aagnone/github/brew/brew/Cellar/portaudio/19.6.0/lib")
#
