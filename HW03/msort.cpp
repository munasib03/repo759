#include "msort.h"

#include <omp.h>

#include <vector>

namespace {

// Serial, non-recursive sort used below the threshold.
void insertion_sort(int *arr, std::size_t n) {
    for (std::size_t i = 1; i < n; ++i) {
        const int key = arr[i];
        std::size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

// Merge sorted arr[0, mid) and arr[mid, n) using tmp (same length) as scratch.
void merge(int *arr, int *tmp, std::size_t mid, std::size_t n) {
    std::size_t i = 0, j = mid, k = 0;
    while (i < mid && j < n) {
        tmp[k++] = (arr[j] < arr[i]) ? arr[j++] : arr[i++];
    }
    while (i < mid) tmp[k++] = arr[i++];
    while (j < n) tmp[k++] = arr[j++];
    for (std::size_t x = 0; x < n; ++x) arr[x] = tmp[x];
}

void msort_task(int *arr, int *tmp, std::size_t n, std::size_t threshold) {
    if (n < 2) return;
    if (n < threshold) {
        insertion_sort(arr, n);
        return;
    }

    const std::size_t mid = n / 2;
    // Left and right halves touch disjoint memory, so they can run as
    // independent tasks. Pointers/sizes are firstprivate by default in a task
    // because they are local (private) in the enclosing context.
#pragma omp task
    msort_task(arr, tmp, mid, threshold);
#pragma omp task
    msort_task(arr + mid, tmp + mid, n - mid, threshold);
#pragma omp taskwait  // both halves must be sorted before merging
    merge(arr, tmp, mid, n);
}

}  // namespace

void msort(int *arr, const std::size_t n, const std::size_t threshold) {
    if (n < 2) return;
    std::vector<int> tmp(n);
    // Threshold of 0/1 would recurse down to single elements; that's still
    // correct, just slow.
#pragma omp parallel
#pragma omp single
    msort_task(arr, tmp.data(), n, threshold);
}
