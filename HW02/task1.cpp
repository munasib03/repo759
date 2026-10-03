#include "scan.h"

#include <charconv>
#include <chrono>
#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <ratio>
#include <string_view>
#include <system_error>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " n (positive integer)\n";
        return 1;
    }

    const std::string_view argument(argv[1]);
    std::size_t n = 0;
    const auto result = std::from_chars(
        argument.data(), argument.data() + argument.size(), n);
    if (result.ec != std::errc{} ||
        result.ptr != argument.data() + argument.size() || n == 0) {
        std::cerr << "n must be a positive integer that fits in std::size_t.\n";
        return 1;
    }

    try {
        // Smart pointers release both arrays automatically when leaving scope.
        std::unique_ptr<float[]> arr(new float[n]);
        std::unique_ptr<float[]> output(new float[n]);

        std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
        for (std::size_t i = 0; i < n; ++i) {
            arr[i] = distribution(generator);
        }

        const auto start = std::chrono::high_resolution_clock::now();
        scan(arr.get(), output.get(), n);
        const auto stop = std::chrono::high_resolution_clock::now();
        const double milliseconds =
            std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
                stop - start).count();

        std::cout << std::setprecision(10)
                  << milliseconds << '\n'
                  << output[0] << '\n'
                  << output[n - 1] << '\n';
    } catch (const std::exception &error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
