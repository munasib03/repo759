#include "msort.h"

#include <omp.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <ratio>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " n t ts\n";
        return 1;
    }
    const long n_in = std::atol(argv[1]);
    const int t = std::atoi(argv[2]);
    const long ts_in = std::atol(argv[3]);
    if (n_in <= 0 || t < 1 || t > 20 || ts_in <= 0) {
        std::cerr << "n, ts must be positive and t must be in [1, 20].\n";
        return 1;
    }
    const std::size_t n = static_cast<std::size_t>(n_in);
    const std::size_t ts = static_cast<std::size_t>(ts_in);

    std::vector<int> arr(n);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(-1000, 1000);
    for (auto &v : arr) v = dist(gen);

    omp_set_num_threads(t);

    const auto start = std::chrono::high_resolution_clock::now();
    msort(arr.data(), n, ts);
    const auto end = std::chrono::high_resolution_clock::now();
    const double ms =
        std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << arr[0] << '\n' << arr[n - 1] << '\n' << ms << '\n';
    return 0;
}
