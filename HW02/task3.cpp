#include "matmul.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <ratio>
#include <vector>

namespace {

constexpr unsigned int matrix_size = 1024;

double elapsed_milliseconds(
    std::chrono::high_resolution_clock::time_point start,
    std::chrono::high_resolution_clock::time_point stop) {
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
               stop - start)
        .count();
}

bool close_enough(double a, double b) {
    const double tolerance = 1.0e-8 * std::max({1.0, std::fabs(a), std::fabs(b)});
    return std::fabs(a - b) <= tolerance;
}

bool matrices_close(
    const std::vector<double> &left, const std::vector<double> &right) {
    for (std::size_t i = 0; i < left.size(); ++i) {
        if (!close_enough(left[i], right[i])) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    const unsigned int n = matrix_size;
    const std::size_t entries = static_cast<std::size_t>(n) * n;

    std::vector<double> A(entries);
    std::vector<double> B(entries);
    std::vector<double> C(entries);
    std::vector<double> reference(entries);

    std::mt19937 generator(759);
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    for (std::size_t i = 0; i < entries; ++i) {
        A[i] = distribution(generator);
        B[i] = distribution(generator);
    }

    double times[4] = {};
    double last_values[4] = {};

    auto start = std::chrono::high_resolution_clock::now();
    mmul1(A.data(), B.data(), C.data(), n);
    auto stop = std::chrono::high_resolution_clock::now();
    times[0] = elapsed_milliseconds(start, stop);
    last_values[0] = C[entries - 1];
    reference = C;

    start = std::chrono::high_resolution_clock::now();
    mmul2(A.data(), B.data(), C.data(), n);
    stop = std::chrono::high_resolution_clock::now();
    times[1] = elapsed_milliseconds(start, stop);
    last_values[1] = C[entries - 1];
    if (!matrices_close(reference, C)) {
        std::cerr << "mmul2 result does not match mmul1.\n";
        return 1;
    }

    start = std::chrono::high_resolution_clock::now();
    mmul3(A.data(), B.data(), C.data(), n);
    stop = std::chrono::high_resolution_clock::now();
    times[2] = elapsed_milliseconds(start, stop);
    last_values[2] = C[entries - 1];
    if (!matrices_close(reference, C)) {
        std::cerr << "mmul3 result does not match mmul1.\n";
        return 1;
    }

    start = std::chrono::high_resolution_clock::now();
    mmul4(A, B, C.data(), n);
    stop = std::chrono::high_resolution_clock::now();
    times[3] = elapsed_milliseconds(start, stop);
    last_values[3] = C[entries - 1];
    if (!matrices_close(reference, C)) {
        std::cerr << "mmul4 result does not match mmul1.\n";
        return 1;
    }

    std::cout << std::setprecision(10) << n << '\n';
    for (int i = 0; i < 4; ++i) {
        std::cout << times[i] << '\n' << last_values[i] << '\n';
    }

    return 0;
}

