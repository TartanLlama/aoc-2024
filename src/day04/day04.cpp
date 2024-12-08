#define MDSPAN_USE_BRACKET_OPERATOR 1
#include <mdspan>
#include <ctre.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <ranges>
#include <generator>
#include <tl/concat.hpp>
#include <tl/fold.hpp>
#include "../coord.hpp"
#include "../grid.hpp"
#include <algorithm>


namespace ext = std::experimental;


void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day04/input.txt" };
    tl::grid<char> grid{ input };

    auto four_length_strings = grid.all_coords()
        | std::views::filter([&](auto coord) { return grid[coord] == 'X'; })
        | std::views::transform([&](auto coord) {
            return grid.slices_in_directions(tl::directions::all, coord);
          })
        | std::views::join
        | std::views::transform([](auto&& slice) {
            return std::move(slice) | std::views::take(4);
          });

    auto xmas_count = std::ranges::count_if(four_length_strings, [](auto&& str) {
        return std::ranges::equal(str, std::string_view("XMAS"));
    });
    std::cout << xmas_count;
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day04/input.txt" };
    tl::grid<char> grid{ input };

    auto has_x_mas = [&](auto coord) {
        return
            grid[coord] == 'A' and
            std::abs(grid[coord.up_left()] - grid[coord.down_right()]) == 6 and
            std::abs(grid[coord.up_right()] - grid[coord.down_left()]) == 6;
    };
    std::cout << std::ranges::count_if(grid.coords_within_border(1), has_x_mas);
}

int main() {
    part2();
}