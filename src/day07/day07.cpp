#include <tl/k_combinations.hpp>
#include "../split.hpp"
#include "../lift.hpp"
#include <tl/adjacent.hpp>
#include <tl/getlines.hpp>
#include <fstream>
#include <iostream>
#include <functional>
#include <format>

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day07/input.txt" };
    auto calibration_results = tl::views::getlines(input)
        | std::views::transform([](auto&& line) {
            auto colon = line.find(':');
            auto expected = std::stoull(line.substr(0, colon));
            auto ns = tl::split<std::uint64_t>(line.substr(colon + 2), " ");
            auto ops = std::array<std::uint64_t(*)(std::uint64_t, std::uint64_t), 2>{
                [](std::uint64_t i, std::uint64_t j) { return i + j; },
                [](std::uint64_t i, std::uint64_t j) { return i * j; }
            };
            auto possible = std::ranges::any_of(tl::views::k_combinations(ops, ns.size() - 1),
                [&](auto&& ops) {
                    auto opit = ops.begin();
                    return tl::fold_left_first(ns,
                        [=](std::uint64_t acc, std::uint64_t x) mutable {
                            return (*opit++)(acc, x);
                        }) == expected;
                });
            return possible ? expected : 0;
        });
    std::cout <<
        tl::sum(calibration_results);
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day07/input.txt" };
    auto calibration_results 
        = tl::views::getlines(input)
        | std::views::transform([](auto&& line) {
            using int_t = std::uint64_t;
            auto colon = line.find(':');
            auto expected = std::stoull(line.substr(0, colon));
            auto ns = tl::split<int_t>(line.substr(colon + 2), " ");
            auto ops = std::array<int_t(*)(int_t, int_t), 3>{
                [](int_t i, int_t j) { return i + j; },
                [](int_t i, int_t j) { return i * j; },
                [](int_t i, int_t j) { return std::stoull(std::format("{}{}", i, j)); }
            };
            auto ops_give_expected_result = [&](auto&& ops) {
                return expected == tl::fold_left_first(ns,
                    [opit = ops.begin()](int_t acc, int_t x) mutable {
                        return (*opit++)(acc, x);
                    });
            };
            auto is_possible = std::ranges::any_of(
                tl::views::k_combinations(ops, ns.size() - 1),
                ops_give_expected_result);
            return is_possible ? expected : 0;
        });
    std::cout << tl::sum(calibration_results);
}

int main() {
    part2();
}

