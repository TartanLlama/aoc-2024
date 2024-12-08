#include "../grid.hpp"
#include <tl/functional/curry.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_set>
#include <numeric>
#include <generator>

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day08/input.txt" };
    tl::grid<char> grid{ input };
    
    auto antennae = grid.all_coords()
        | std::views::filter([&](auto c) { return grid[c] != '.'; })
        | std::ranges::to<std::vector>();
    auto antinodes = std::views::cartesian_product(antennae, antennae)
        | std::views::filter(tl::uncurry([&](auto a, auto b) {
            return a != b and grid[a] == grid[b];
          }))
        | std::views::transform(tl::uncurry([&](auto a, auto b) {
            auto diff = b - a;
            return std::array{ a - diff, b + diff };
          }))
        | std::views::join
        | std::views::filter([&](auto c) { return grid.in_bounds(c); })
        | std::ranges::to<std::unordered_set>();
    std::cout << antinodes.size();
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day08/input.txt" };
    tl::grid<char> grid{ input };

    auto antennae = grid.all_coords()
        | std::views::filter([&](auto c) { return grid[c] != '.'; })
        | std::ranges::to<std::vector>();
    auto antinodes = std::views::cartesian_product(antennae, antennae)
        | std::views::filter(tl::uncurry([&](auto a, auto b) {
            return a != b and grid[a] == grid[b];
          }))
        | std::views::transform(tl::uncurry([&](auto a, auto b) -> std::generator<tl::coord> {
            auto unit = b - a;
            while (grid.in_bounds(a)) {
                co_yield a;
                a -= unit;
            }
            while (grid.in_bounds(b)) {
                co_yield b;
                b += unit;
            }
          }))
        | std::views::join
        | std::ranges::to<std::unordered_set>();
    std::cout << antinodes.size();
}

int main() {
    part2();
}