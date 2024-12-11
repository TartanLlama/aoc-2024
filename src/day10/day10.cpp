#include <fstream>
#include <iostream>
#include "../grid.hpp"
#include <tl/fold.hpp>
#include <unordered_set>
#include "../lift.hpp"

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day10/input.txt" };
    tl::grid<int, tl::mark_out_of_bounds_with<-1>> grid{ input };

    auto score_trailhead = [](this auto self, auto c, auto& grid) -> std::unordered_set<tl::coord> {
        if (grid[c] == 9) {
            return { c };
        }
        auto all_summits = tl::directions::orthogonals
            | std::views::filter([&](auto d) { return grid[c + d] - grid[c] == 1; })
            | std::views::transform([&](auto d) { return self(c + d, grid); })
            | std::views::join;
        return { std::from_range, all_summits };
        };

    auto scores = grid.all_coords()
        | std::views::filter([&](auto c) { return grid[c] == 0; })
        | std::views::transform(std::bind_back(score_trailhead, grid));
    std::cout << tl::fold(scores, 0, [](auto acc, auto&& set) { return acc + set.size(); });
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day10/input.txt" };
    tl::grid<int, tl::mark_out_of_bounds_with<-1>> grid{ input };

    auto score_trailhead = [&](this auto self, auto c) {
        if (grid[c] == 9) {
            return 1;
        }
        auto scores = tl::directions::orthogonals
            | std::views::filter([&](auto d) { return grid[c + d] - grid[c] == 1; })
            | std::views::transform([&](auto d) { return self(c + d); });
        return tl::sum(scores);
    };

    auto scores = grid.all_coords()
        | std::views::filter([&](auto c) { return grid[c] == 0; })
        | std::views::transform(score_trailhead);
    std::cout << tl::sum(scores);
}

int main() {
    part2();
}