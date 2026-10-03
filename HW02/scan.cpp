#include "scan.h"

void scan(const float *arr, float *output, std::size_t n) {
    if (n == 0) {
        return;
    }

    float sum = arr[0];
    output[0] = sum;
    for (std::size_t i = 1; i < n; ++i) {
        sum += arr[i];
        output[i] = sum;
    }
}

