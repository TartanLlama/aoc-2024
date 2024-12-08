#include <fstream>
#include <iostream>
#include "../grid.hpp"
#include <ranges>
#include <set>
#include <unordered_set>
#include <generator>
#include <tl/adjacent.hpp>
#include <tl/cache_latest.hpp>
#include <algorithm>
#include <tl/functional/curry.hpp>
#include "../lift.hpp"

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day06/input.txt" };
    tl::grid<char> grid{ input };
    auto coords = grid.all_coords();
    auto guard = *std::ranges::find_if(coords, [&](auto coord) {
        return grid[coord] == '^';
        });
    auto direction = tl::directions::up;
    std::unordered_set<tl::coord> visited;
    while (grid.in_bounds(guard + direction)) {
        visited.insert(guard);
        if (grid[guard + direction] == '#') {
            direction.turn_right();
        }
        else {
            guard += direction;
        }
    }
    
    std::cout << visited.size() + 1;
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day06/input.txt" };
    tl::grid<char> grid{ input };
    auto coords = grid.all_coords();
    auto guard = *std::ranges::find_if(coords, [&](auto coord) {
        return grid[coord] == '^';
        });
    grid[guard] = '.';

    auto direction = tl::directions::up;
    auto check_loop = [&grid](auto guard, auto direction) {
        std::set<std::pair<tl::coord, tl::coord>> visited;

        while (grid.in_bounds(guard + direction)) {
            if (grid[guard + direction] == '#') {
                if (auto [_, inserted] = visited.insert(std::pair(guard, direction)); 
                    !inserted) {
                    return true;
                }
                direction.turn_right();
            }
            else {
                guard += direction;
            }
        }
        return false;
    };

    auto pos = guard;
    std::set<tl::coord> visited;
    while (grid.in_bounds(pos + direction)) {
        visited.insert(pos);
        auto ahead = pos + direction;
        if (grid[ahead] == '#') {
            direction.turn_right();
        }
        else {
            pos = ahead;
        }
    }

    int loop_count = 0;
    for (auto coord : visited) {
        grid[coord] = '#';
        if (check_loop(guard, tl::directions::up)) {
            loop_count++;
        }
        grid[coord] = '.';
    }
    std::cout << loop_count;
}

int main() {
    part2();
}