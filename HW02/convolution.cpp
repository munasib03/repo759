#include "convolution.h"

namespace {

float image_value(const float *image, std::size_t n, long row, long col) {
    const bool row_inside = 0 <= row && row < static_cast<long>(n);
    const bool col_inside = 0 <= col && col < static_cast<long>(n);

    if (row_inside && col_inside) {
        return image[static_cast<std::size_t>(row) * n +
                     static_cast<std::size_t>(col)];
    }
    if (row_inside || col_inside) {
        return 1.0f;
    }
    return 0.0f;
}

}  // namespace

void convolve(
    const float *image, float *output, std::size_t n, const float *mask,
    std::size_t m) {
    const long half = static_cast<long>(m / 2);

    for (std::size_t row = 0; row < n; ++row) {
        for (std::size_t col = 0; col < n; ++col) {
            float sum = 0.0f;
            for (std::size_t mask_row = 0; mask_row < m; ++mask_row) {
                for (std::size_t mask_col = 0; mask_col < m; ++mask_col) {
                    const long image_row = static_cast<long>(row) +
                                           static_cast<long>(mask_row) - half;
                    const long image_col = static_cast<long>(col) +
                                           static_cast<long>(mask_col) - half;
                    sum += mask[mask_row * m + mask_col] *
                           image_value(image, n, image_row, image_col);
                }
            }
            output[row * n + col] = sum;
        }
    }
}

