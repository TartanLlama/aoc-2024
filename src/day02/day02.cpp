#include <ranges>
#include <tl/getlines.hpp>
#include <fstream>
#include <tl/slide.hpp>
#include "../split.hpp"
#include <tl/concat.hpp>
#include <tl/repeat_n.hpp>
#include <tl/functional/curry.hpp>
#include <tl/adjacent.hpp>
#include <tl/adjacent_transform.hpp>

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day02/input.txt" };
    auto safeties = tl::views::getlines(input)
        | std::views::transform([](auto& line) {
        auto split = tl::split<int>(line, ' ');
        auto direction_mod = split[0] < split[1] ? 1 : -1;
        auto valid_pairs = split
            | tl::views::slide(2)
            | std::views::transform([=](auto&& chunk) {
            auto distance = (chunk[1] - chunk[0]) * direction_mod;
            return distance > 0 and distance <= 3;
                });
        return std::ranges::all_of(valid_pairs, std::identity{});
            });
    std::cout << std::ranges::count(safeties, true);
}

namespace tl {
    auto drop_at = [](auto&& rng, auto idx) {
        return tl::views::concat(
            rng | std::views::take(idx),
            rng | std::views::drop(idx + 1)
        ) | std::ranges::to<std::vector>();
        };
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day02/input.txt" };
    auto is_safe = [](auto&& levels) {
        auto direction_mod = levels[0] < levels[1] ? 1 : -1;
        auto valid_pairs
            = levels
            | tl::views::pairwise_transform([=](auto l, auto r) {
                auto distance = (r - l) * direction_mod;
                return distance > 0 and distance <= 3;
              });
        return std::ranges::all_of(valid_pairs, std::identity{});
    };

    auto is_safe_with_tolerance = [&](auto&& levels) {
        if (is_safe(levels)) {
            return true;
        }
        auto dampened_safeties
            = std::views::iota(std::size_t(0), levels.size())
            | std::views::transform([&](auto idx) {
                return is_safe(tl::drop_at(levels, idx));
              });
        return std::ranges::any_of(dampened_safeties, std::identity{});
    };

    auto safe_lines
        = tl::views::getlines(input)
        | std::views::transform(std::bind_back(tl::split<int>, ' '))
        | std::views::transform(is_safe_with_tolerance);

    std::cout << std::ranges::count(safe_lines, true);
}

int main() {
    part2();
}

/*
1 2 1
2 4 2
4 7 3
7 9 2
9 8 -1

2 4 2
4 7 3
7 9 2
9 8 -1

1 4 3
4 7 3
7 9 2
9 8 -1

1 2 1
2 7 5

1 2 1
2 4 2
4 9 5

1 2 1
2 4 2
4 7 3
7 8 1*/