#include "matmul.h"

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

    std::vector<float> A(n * n), B(n * n), C(n * n);
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < n * n; ++i) {
        A[i] = dist(gen);
        B[i] = dist(gen);
    }

    omp_set_num_threads(t);

    const auto start = std::chrono::high_resolution_clock::now();
    mmul(A.data(), B.data(), C.data(), n);
    const auto end = std::chrono::high_resolution_clock::now();
    const double ms =
        std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << C[0] << '\n' << C[n * n - 1] << '\n' << ms << '\n';
    return 0;
}
