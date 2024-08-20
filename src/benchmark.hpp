#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <utility>

//==============================================================================
// Macro tests.
#define BENCH_START(name) \
    auto bench_start_##name = std::chrono::high_resolution_clock::now(); \
    int bench_iters_total_##name = 0; \
    std::cout << "\n====== Bench " << #name << " started =======\n";

#define BENCH_STOP(name) \
    auto bench_end_##name = std::chrono::high_resolution_clock::now(); \
    auto bench_duration_us_##name = std::chrono::duration_cast<std::chrono::microseconds>(bench_end_##name - bench_start_##name).count(); \
    auto bench_duration_ms_##name = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(bench_end_##name - bench_start_##name).count();

#define BENCH_LOG(name) \
    std::cout << "\n====== Bench " << #name << " has ended =======\n"; \
    std::cout << std::fixed << std::setprecision(3); \
    std::cout << "\t" << #name << " took " << bench_duration_us_##name << " us microseconds" << std::endl;\
    std::cout << "\t" << #name << " took " << bench_duration_ms_##name << " ms milliseconds." << std::endl;

//==============================================================================
// Version 2 of benchmarker. Now using more ++ than C.
class Benchmark{
    private:
        std::string name;
        int iter_count;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
        std::chrono::duration<double> total_duration;
    public:
        Benchmark(std::string name, int iter_count) : name(std::move(name)), iter_count(iter_count), total_duration(0) {}

        void start() {
            std::cout << "\n====== Bench " << name << " has started =======\n";
            start_time = std::chrono::high_resolution_clock::now();
        }

        // void setIterCount(int new_count) { iter_count = new_count; }

        // It is what it is. It is only meant to be used in this project, so I have
        // to resist the temptation to overenginner it
        void stop(int iters=1) {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
            total_duration += duration;

            auto avg = total_duration / iters;
            iter_count = iters;
        }

        void results() {
            double sec_tot = std::chrono::duration_cast<std::chrono::duration<double>>(total_duration).count();
            double sec_it= sec_tot / (double)iter_count;
            double ms_tot = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(total_duration).count(); // Yes, I know its sec_tot / 1000 but I wanted to write it _prettier_ (to understand chrono a bit)
            double ms_it= ms_tot / (double)iter_count;
            double us_tot = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(total_duration).count();
            double us_it= us_tot / (double)iter_count;
            std::cout << "\n====== Bench " << name << " has ended =======\n";
            std::cout << std::fixed << std::setprecision(3);
            std::cout << "\t" << name << " took: " << std::setw(10) << std::right << sec_tot << "s (total)   |  " << std::setw(10) << std::right << sec_it << "s (per iteration).\n";
            std::cout << "\t" << name << " took: " << std::setw(10) << std::right << ms_tot << "ms (total)  |  " << std::setw(10) << std::right << ms_it << "ms (per iteration).\n";
            std::cout << "\t" << name << " took: " << std::setw(10) << std::right << us_tot << "us (total)  |  " << std::setw(10) << std::right << us_it << "us (per iteration).\n";
            std::cout << "\t" << name << " iters " <<  iter_count << ". \n";
            std::cout << "==================================\n";

        }
};