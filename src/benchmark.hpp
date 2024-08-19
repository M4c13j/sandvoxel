#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>

#define BENCHMARK_START(name) \
    auto benchmark_start_##name = std::chrono::high_resolution_clock::now(); \
    std::cout << "\n====== Benchmark " << #name << " started =======\n";

#define BENCHMARK_STOP(name) \
    auto benchmark_end_##name = std::chrono::high_resolution_clock::now(); \
    auto benchmark_duration_us_##name = std::chrono::duration_cast<std::chrono::microseconds>(benchmark_end_##name - benchmark_start_##name).count(); \
    auto benchmark_duration_ms_##name = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(benchmark_end_##name - benchmark_start_##name).count();

#define BENCHMARK_LOG(name) \
    std::cout << "\n====== Benchmark " << #name << " has ended =======\n"; \
    std::cout << std::fixed << std::setprecision(3); \
    std::cout << "\t" << #name << " took " << benchmark_duration_us_##name << " us microseconds" << std::endl;\
    std::cout << "\t" << #name << " took " << benchmark_duration_ms_##name << " ms milliseconds." << std::endl;

