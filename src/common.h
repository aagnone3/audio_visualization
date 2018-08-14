#pragma once
#include <iostream>

#define OUT(stream_text) std::cout << stream_text << std::endl
#define EPSILON -1e-7f
#define FPS 60.0f
#define DEBUG 1.0

using namespace std;
template <typename T>
void zeros(T* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
}
