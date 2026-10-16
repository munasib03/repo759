#include "matmul.h"

#include <omp.h>

// Parallel version of HW02 mmul2 (i-k-j loop order).
// Each thread owns a disjoint set of rows i of C, so no synchronization is needed.
void mmul(const float *A, const float *B, float *C, const std::size_t n) {
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < n; ++i) {
        float *c_row = C + i * n;
        for (std::size_t j = 0; j < n; ++j) {
            c_row[j] = 0.0f;
        }
        for (std::size_t k = 0; k < n; ++k) {
            const float a_ik = A[i * n + k];
            const float *b_row = B + k * n;
            for (std::size_t j = 0; j < n; ++j) {
                c_row[j] += a_ik * b_row[j];
            }
        }
    }
}
