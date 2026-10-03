#include "convolution.h"

#include <charconv>
#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <ratio>
#include <string_view>
#include <system_error>

namespace {

bool parse_positive_size(const char *text, std::size_t &value) {
    const std::string_view argument(text);
    value = 0;
    const auto result = std::from_chars(
        argument.data(), argument.data() + argument.size(), value);
    return result.ec == std::errc{} &&
           result.ptr == argument.data() + argument.size() && value > 0;
}

bool square_would_overflow(std::size_t value) {
    return value > std::numeric_limits<std::size_t>::max() / value;
}

}  // namespace

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " n m (positive integers, m odd)\n";
        return 1;
    }

    std::size_t n = 0;
    std::size_t m = 0;
    if (!parse_positive_size(argv[1], n) || !parse_positive_size(argv[2], m) ||
        m % 2 == 0) {
        std::cerr << "n and m must be positive integers, and m must be odd.\n";
        return 1;
    }
    if (square_would_overflow(n) || square_would_overflow(m)) {
        std::cerr << "Matrix dimensions are too large.\n";
        return 1;
    }

    const std::size_t image_size = n * n;
    const std::size_t mask_size = m * m;

    float *image = nullptr;
    float *mask = nullptr;
    float *output = nullptr;

    try {
        image = new float[image_size];
        mask = new float[mask_size];
        output = new float[image_size];

        std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<float> image_distribution(
            -10.0f, 10.0f);
        std::uniform_real_distribution<float> mask_distribution(-1.0f, 1.0f);

        for (std::size_t i = 0; i < image_size; ++i) {
            image[i] = image_distribution(generator);
        }
        for (std::size_t i = 0; i < mask_size; ++i) {
            mask[i] = mask_distribution(generator);
        }

        const auto start = std::chrono::high_resolution_clock::now();
        convolve(image, output, n, mask, m);
        const auto stop = std::chrono::high_resolution_clock::now();
        const double milliseconds =
            std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
                stop - start).count();

        std::cout << std::setprecision(10)
                  << milliseconds << '\n'
                  << output[0] << '\n'
                  << output[image_size - 1] << '\n';
    } catch (const std::exception &error) {
        delete[] image;
        delete[] mask;
        delete[] output;
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    delete[] image;
    delete[] mask;
    delete[] output;
    return 0;
}

