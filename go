#!/bin/bash
pushd build
cmake ..
make
rc=$?
popd
[[ $rc -eq 0 ]] && build/opengl_spectrogram
