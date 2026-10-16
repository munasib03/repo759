#include "convolution.h"

#include <omp.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <ratio>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " n t\n";
        return 1;
    }
    const long n_in = std::atol(argv[1]);
    const int t = std::atoi(argv[2]);
    if (n_in <= 0 || t < 1 || t > 20) {
        std::cerr << "n must be positive and t must be in [1, 20].\n";
        return 1;
    }
    const std::size_t n = static_cast<std::size_t>(n_in);
    const std::size_t m = 3;

    std::vector<float> image(n * n), output(n * n), mask(m * m);
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> image_dist(-10.0f, 10.0f);
    std::uniform_real_distribution<float> mask_dist(-1.0f, 1.0f);
    for (auto &v : image) v = image_dist(gen);
    for (auto &v : mask) v = mask_dist(gen);

    omp_set_num_threads(t);

    const auto start = std::chrono::high_resolution_clock::now();
    convolve(image.data(), output.data(), n, mask.data(), m);
    const auto end = std::chrono::high_resolution_clock::now();
    const double ms =
        std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << output[0] << '\n' << output[n * n - 1] << '\n' << ms << '\n';
    return 0;
}
