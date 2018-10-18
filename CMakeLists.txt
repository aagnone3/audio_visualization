cmake_minimum_required(VERSION 3.5)
project(AudioVisualization)


# ============================
# target executable specification
# ============================
add_executable(opengl_spectrogram
    src/AudioInput.cpp
    src/Display.cpp
    src/Log.cpp
    src/PortAudio.cpp
    src/SpectrogramVisualizer.cpp
    src/main.cpp
    src/shared.cpp
)
add_executable(test_input src/util/testInput.cpp)
add_executable(device_info src/util/showAllDeviceInfo.cpp)
set(EXEC_TARGETS opengl_spectrogram test_input device_info)


# ============================
# versioning
# ============================
message(${PROJECT_BINARY_DIR})
set(AudioVisualization_VERSION_MAJOR 1)
set(AudioVisualization_VERSION_MINOR 2)
configure_file(
    "${PROJECT_SOURCE_DIR}/src/AudioVisualizationConfig.h.in"
    "${PROJECT_BINARY_DIR}/AudioVisualizationConfig.h"
)
include_directories("${PROJECT_BINARY_DIR}")


# ============================
# dependencies and compilation
# ============================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/Modules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra --std=c++14 -g")

# LibFFTW3
find_package(LibFFTW3 REQUIRED MODULE)
include_directories(${FFTW3_INCLUDES})
set(LIBS ${LIBS} ${FFTW3_LIBRARIES})

# LibPortAudio
find_package(LibPortAudio REQUIRED MODULE)
include_directories(${PORTAUDIO_INCLUDES})
set(LIBS ${LIBS} ${PORTAUDIO_LIBRARIES})

# OpenGL and GLUT
find_package(OpenGL REQUIRED MODULE)
find_package(GLUT REQUIRED MODULE)
include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

foreach(target ${EXEC_TARGETS})
    target_link_libraries(${target} ${LIBS})
endforeach()


# ============================
# installation
# ============================
install(
    TARGETS
        opengl_spectrogram
        test_input
        device_info
    DESTINATION
        bin
)
install(
    FILES
        ${PROJECT_BINARY_DIR}/AudioVisualizationConfig.h
    DESTINATION
        include
)


# ============================
# testing
# ============================
include(CTest)
add_test(TestTestInput test_input)
add_test(TestDeviceInfo device_info)


# ============================
# packaging
# ============================
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_VERSION_MAJOR ${AudioVisualization_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${AudioVisualization_VERSION_MINOR})
include(CPack)
