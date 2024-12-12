#include <tl/getlines.hpp>
#include <fstream>
#include <iostream>
#include "../split.hpp"
#include <tl/fold.hpp>
#include <cmath>
#include <chrono>
#include "../unordered_dense.h"

std::size_t count_stones(const std::string& line, std::size_t blinks) {
    ankerl::unordered_dense::map<std::uint64_t, std::size_t> cache;
    auto simulate_blinks = [&](this auto self, std::uint64_t n, std::uint8_t blinks) -> std::size_t {
        if (blinks == 0) {
            return 1;
        }
        auto key = (n << 8) | blinks;
        if (cache.contains(key)) {
            return cache[key];
        }
        if (n == 0) {
            return cache[key] = self(1, blinks - 1);
        }
        if (int n_digits = std::floor(std::log10(n) + 1); n_digits % 2 == 0) {
            int half_magnitude = std::pow(10, n_digits / 2);
            return cache[key] = 
                self(n / half_magnitude, blinks - 1) + self(n % half_magnitude, blinks - 1);
        }
        return cache[key] = self(n * 2024, blinks - 1);
    };
    auto counts = tl::split<int>(line, ' ') 
                | std::views::transform(std::bind_back(simulate_blinks, blinks));
    return tl::sum(counts);
}


int main() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day11/input.txt" };
    std::string line;
    std::getline(input, line);
    
    std::cout << count_stones(line,25) << '\n';
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << count_stones(line,75) << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n";
}