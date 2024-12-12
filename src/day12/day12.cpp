#include <iostream>
#include <fstream>
#include "../grid.hpp"
#include <unordered_set>
#include <tl/fold.hpp>
#include <set>

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day12/input.txt" };
    tl::grid<int, tl::mark_out_of_bounds_with<-1>> grid{ input };
    std::vector<std::vector<tl::coord>> regions;
    std::unordered_set<tl::coord> visited;
    std::unordered_map<tl::coord, int> walls;

    auto build_region = [&](this auto self, auto& region, auto c) -> void {
        region.push_back(c);
        visited.insert(c);
        for (auto n : c.orthogonal_neighbours()) {
            if (grid.in_bounds(n) and grid[n] == grid[c] and !visited.contains(n)) {
                self(region, n);
            }
            if (grid[n] != grid[c]) {
                walls[c]++;
            }
        }
    };
    for (auto c : grid.all_coords()) {
        if (visited.contains(c)) {
            continue;
        }
        std::vector<tl::coord> region;
        build_region(region, c);
        regions.push_back(std::move(region));
    }
    auto prices = regions | std::views::transform([&](auto& region) {
        auto n_walls = region | std::views::transform([&](auto c) { return walls[c]; });
        return tl::sum(n_walls) * region.size();
    });
    std::cout << tl::sum(prices);
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day12/input.txt" };
    tl::grid<int, tl::mark_out_of_bounds_with<-1>> grid{ input };
    std::vector<std::vector<tl::coord>> regions;
    std::unordered_set<tl::coord> visited;

    auto build_region = [&](this auto self, auto& region, auto c) -> void {
        region.push_back(c);
        visited.insert(c);
        for (auto n : c.orthogonal_neighbours()) {
            if (grid.in_bounds(n) and grid[n] == grid[c] and !visited.contains(n)) {
                self(region, n);
            }
        }
    };
    for (auto c : grid.all_coords()) {
        if (visited.contains(c)) {
            continue;
        }
        std::vector<tl::coord> region;
        build_region(region, c);
        regions.push_back(std::move(region));
    }
    int price = 0;
    for (auto& region : regions) {
        std::ranges::sort(region);
        int sides = 0;
        std::set<std::pair<tl::coord, tl::coord>> walls;
        for (auto c : region) {
            for (auto d : tl::directions::orthogonals) {
                auto n = c + d;
                if (grid[n] != grid[c] and !walls.contains({c,d})) {
                    walls.insert({ c, d });
                    auto turned = d.x() == 0 ? tl::coord(1, 0) : tl::coord(0, 1);
                    auto wall = c + turned;
                    while (grid[wall] == grid[c] and grid[wall + d] != grid[wall]) {
                        walls.insert({ wall, d });
                        wall += turned;
                    }
                    ++sides;
                }
            }
        }
        auto new_price = sides * region.size();
        price += new_price;
    }
    std::cout << price;
}

void main() {
    part2();
}