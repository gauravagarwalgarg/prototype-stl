///////////////////////////////////////////////////////////////////////////////
// benchmarks/bench_vector.cpp
//
// PrototypeSTL - Vector performance benchmark
//
// Compares: prototype::vector vs prototype::fixed_vector vs std::vector
//
///////////////////////////////////////////////////////////////////////////////

#include "../prototype_config.hpp"
#include "../prototype_vector.hpp"
#include <stdio.h>
#include <time.h>

// Simple high-resolution timer
static inline double get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
}

static const size_t ITERATIONS = 100000;
static const size_t ELEMENTS = 1000;

void bench_prototype_vector() {
    double start = get_time_ns();
    for (size_t iter = 0; iter < ITERATIONS; ++iter) {
        prototype::vector<int> v;
        v.reserve(ELEMENTS);
        for (size_t i = 0; i < ELEMENTS; ++i) {
            v.push_back(static_cast<int>(i));
        }
        // Random access
        volatile int sum = 0;
        for (size_t i = 0; i < ELEMENTS; ++i) {
            sum += v[i];
        }
        (void)sum;
    }
    double elapsed = get_time_ns() - start;
    printf("  prototype::vector:       %.2f ms (%zu iters x %zu elements)\n",
           elapsed / 1e6, ITERATIONS, ELEMENTS);
}

void bench_fixed_vector() {
    double start = get_time_ns();
    for (size_t iter = 0; iter < ITERATIONS; ++iter) {
        prototype::fixed_vector<int, 1024> v;
        for (size_t i = 0; i < ELEMENTS; ++i) {
            v.push_back(static_cast<int>(i));
        }
        volatile int sum = 0;
        for (size_t i = 0; i < ELEMENTS; ++i) {
            sum += v[i];
        }
        (void)sum;
    }
    double elapsed = get_time_ns() - start;
    printf("  prototype::fixed_vector: %.2f ms (%zu iters x %zu elements)\n",
           elapsed / 1e6, ITERATIONS, ELEMENTS);
}

void bench_push_pop() {
    printf("\n  Push/Pop benchmark (1M ops):\n");
    const size_t OPS = 1000000;

    {
        prototype::vector<int> v;
        v.reserve(OPS);
        double start = get_time_ns();
        for (size_t i = 0; i < OPS; ++i) v.push_back(static_cast<int>(i));
        for (size_t i = 0; i < OPS; ++i) v.pop_back();
        double elapsed = get_time_ns() - start;
        printf("    prototype::vector push+pop: %.2f ms\n", elapsed / 1e6);
    }

    {
        prototype::fixed_vector<int, 1048576> v;
        double start = get_time_ns();
        for (size_t i = 0; i < OPS; ++i) v.push_back(static_cast<int>(i));
        for (size_t i = 0; i < OPS; ++i) v.pop_back();
        double elapsed = get_time_ns() - start;
        printf("    prototype::fixed_vector push+pop: %.2f ms\n", elapsed / 1e6);
    }
}

int main() {
    printf("=== PrototypeSTL Vector Benchmark ===\n\n");
    printf("  Fill + sequential access:\n");

    bench_prototype_vector();
    bench_fixed_vector();
    bench_push_pop();

    printf("\nDone.\n");
    return 0;
}
