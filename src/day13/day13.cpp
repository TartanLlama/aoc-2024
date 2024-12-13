#include <fstream>
#include <iostream>
#include <tl/chunk.hpp>
#include <tl/getlines.hpp>
#include <ctre.hpp>
#include <tl/fold.hpp>
#include <tl/utility/tuple_utils.hpp>
void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day13/input.txt" };
    auto lines = tl::views::getlines(input) | std::ranges::to<std::vector>();

    auto determinant = [](int x1, int y1, int x2, int y2) {
        return x1 * y2 - x2 * y1;
    };
    auto cramer_solve = [&](int a1, int b1, int a2, int b2, int d1, int d2) {
        auto det = determinant(a1, b1, a2, b2);
        auto det_x = determinant(d1, b1, d2, b2);
        auto det_y = determinant(a1, d1, a2, d2);
        return std::pair{ det_x / det, det_y / det };
    };

    auto token_uses = lines | tl::views::chunk(4) | std::views::transform([&](auto&& chunk) {
        constexpr auto regex = ctll::fixed_string(R"(.+: X.(\d+), Y.(\d+))");
        auto it = std::ranges::begin(chunk);
        auto [w1, x1m, y1m] = ctre::match<regex>(*it++);
        auto [w2, x2m, y2m] = ctre::match<regex>(*it++);
        auto [wr, xrm, yrm] = ctre::match<regex>(*it++);
        auto [x1,x2,y1,y2,xr,yr] = tl::tuple_transform([](auto match) { return match.to_number(); }, std::tuple(x1m, x2m, y1m, y2m, xrm, yrm));
        auto [a_times, b_times] = cramer_solve(x1,x2,y1,y2,xr,yr);
        if (x1 * a_times + x2 * b_times != xr or y1 * a_times + y2 * b_times != yr) {
            return 0;
        }
        auto res = a_times * 3 + b_times;
        return res;
    });
    std::cout << tl::sum(token_uses);
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day13/input.txt" };
    auto lines = tl::views::getlines(input) | std::ranges::to<std::vector>();

    using int_t = std::int64_t;
    auto cramer_solve = [](int_t a1, int_t b1, int_t a2, int_t b2, int_t d1, int_t d2) {
        auto det = a1 * b2 - a2 * b1;
        auto det_x = d1 * b2 - d2 * b1;
        auto det_y = a1 * d2 - a2 * d1;
        return std::pair{ det_x / det, det_y / det };
    };
    auto calculate_tokens = [&](auto&& chunk) {
        constexpr auto regex = ctll::fixed_string(R"(.+: X.(\d+), Y.(\d+))");
        auto [_w1, x1m, y1m] = ctre::match<regex>(chunk[0]);
        auto [_w2, x2m, y2m] = ctre::match<regex>(chunk[1]);
        auto [_wr, xrm, yrm] = ctre::match<regex>(chunk[2]);
        auto [x1, x2, y1, y2, xr, yr]
            = tl::tuple_transform([](auto match) -> int_t { return match.to_number(); },
                std::tuple(x1m, x2m, y1m, y2m, xrm, yrm));
        xr += 10000000000000;
        yr += 10000000000000;
        auto [a_times, b_times] = cramer_solve(x1, x2, y1, y2, xr, yr);
        if (x1 * a_times + x2 * b_times != xr or y1 * a_times + y2 * b_times != yr) {
            return int_t(0);
        }
        return a_times * 3 + b_times;
    };

    auto token_uses = lines | tl::views::chunk(4) | std::views::transform(calculate_tokens);
    std::cout << tl::sum(token_uses);
}

int main() {
    part2();
}