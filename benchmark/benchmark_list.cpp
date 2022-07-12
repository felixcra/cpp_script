#include "list.hpp"

#include <iostream>
#include <chrono>
#include <vector>

using namespace cs;

int main() {
    const uint64_t N = 10000000;

    std::cout << "C++ List" << std::endl;
    auto start = std::chrono::steady_clock::now();
    List l;
    for (size_t i = 0; i < N; ++i) {
        l.append(i);
    }
    auto finish            = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
    std::cout << "List insertion :\t" << elapsed_seconds << "s" << std::endl;

    start        = std::chrono::steady_clock::now();
    uint64_t sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += l[i].as<Int>().get();
    }
    finish          = std::chrono::steady_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
    std::cout << "Summation :\t\t" << elapsed_seconds << "s" << std::endl;

    std::cout << "C++ vector<int>" << std::endl;
    start = std::chrono::steady_clock::now();
    vector<int> v;
    for (size_t i = 0; i < N; ++i) {
        v.emplace_back(i);
    }
    finish          = std::chrono::steady_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
    std::cout << "List insertion :\t" << elapsed_seconds << "s" << std::endl;

    start = std::chrono::steady_clock::now();
    sum   = 0;
    for (size_t i = 0; i < N; ++i) {
        sum += v[i];
    }
    finish          = std::chrono::steady_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double> >(finish - start).count();
    std::cout << "Summation :\t\t" << elapsed_seconds << "s" << std::endl;
}
