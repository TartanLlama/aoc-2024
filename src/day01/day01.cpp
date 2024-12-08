#include "../split.hpp"
#include <ranges>
#include <fstream>
#include "../lift.hpp"
#include <tl/getlines.hpp>
#include <tl/fold.hpp>
#include <tl/functional/curry.hpp>
#include <print>


void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day01/input.txt" };
    auto split = tl::views::getlines(input)
        | std::views::transform(
            [](auto s) { return tl::split<int, 2>(s, "   "); })
        | std::ranges::to<std::vector>();

    auto left = split
        | std::views::transform(LIFT(std::get<0>))
        | std::ranges::to<std::vector>();
    std::ranges::sort(left);

    auto right = split
        | std::views::transform(LIFT(std::get<1>))
        | std::ranges::to<std::vector>();
    std::ranges::sort(right);

    auto distances = std::views::zip_transform(
        [](int l, int r) { return std::abs(l - r); }, left, right);
    std::cout << tl::sum(distances);
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day01/input.txt" };
    auto split = tl::views::getlines(input)
        | std::views::transform(
            [](auto s) { return tl::split<int, 2>(s, "   "); })
        | std::ranges::to<std::vector>();

    auto left = split
        | std::views::transform(LIFT(std::get<0>));

    auto right = split
        | std::views::transform(LIFT(std::get<1>))
        | std::ranges::to<std::vector>();

    auto similarities = std::views::transform(left, [&](int n) {
        return n * std::ranges::count(right, n);
    });
    std::cout << tl::sum(similarities);
}


int main() {
    part2();
}