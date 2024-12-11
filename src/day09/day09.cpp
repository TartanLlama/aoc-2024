#include <fstream>
#include <iostream>
#include <string>
#include <ranges>
#include <tl/repeat_n.hpp>
#include <tl/functional/predicates.hpp>
#include <tl/fold.hpp>
#include <tl/functional/curry.hpp>

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day09/input.txt" };
    std::string disk_map;
    std::getline(input, disk_map);

    auto disk
        = disk_map
        | std::views::transform([is_space = false, id = 0ll](char c) mutable {
            auto size = c - '0';
            if (is_space) {
                is_space = false;
                return tl::views::repeat_n(-1ll, size);
            }
            else {
                is_space = true;
                return tl::views::repeat_n(id++, size);
            }
          })
        | std::views::join
        | std::ranges::to<std::vector>();

    auto spaces = disk | std::views::filter(tl::eq(-1));
    auto space_it = std::ranges::begin(spaces);
    auto files = disk | std::views::reverse | std::views::filter(tl::neq(-1));
    auto file_it = std::ranges::begin(files);
    while (space_it.base() < file_it.base().base()) {
        std::swap(*space_it++, *file_it++);
    }

    auto checksum_values
        = disk
        | std::views::take_while(tl::neq(-1))
        | std::views::enumerate
        | std::views::transform(tl::uncurry(std::multiplies{}));
    std::cout << tl::sum(checksum_values);
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day09/input.txt" };
    std::string disk_map;
    std::getline(input, disk_map);

    struct block {
        std::size_t offset;
        std::uint8_t size;
        std::int64_t id;
    };
    std::vector<block> free;
    std::vector<block> used;

    bool is_space = false;
    std::uint64_t id = 0;
    std::size_t offset = 0;
    for (auto c : disk_map) {
        auto size = c - '0';
        auto current_offset = std::exchange(offset, offset + size);
        if (is_space) {
            is_space = false;
            free.emplace_back(current_offset, size, -1ll);
        }
        else {
            is_space = true;
            used.emplace_back(current_offset, size, id++);
        }
    }
    auto used_it = rbegin(used);
    while (used_it != rend(used)) {
        auto free_it = begin(free);
        while (free_it->offset < used_it->offset and free_it->size < used_it->size) {
            ++free_it;
        }
        if (free_it->offset < used_it->offset) {
            free_it->size -= used_it->size;
            used_it->offset = free_it->offset;
            free_it->offset += used_it->size;
        }
        ++used_it;
    }

    auto checksum_values
        = used
        | std::views::transform([](auto b) {
        auto ns
            = std::views::iota(b.offset, b.offset + b.size)
            | std::views::transform(std::bind_back(std::multiplies{}, b.id));
        return tl::sum(ns);
            });
    std::cout << tl::sum(checksum_values);
}

int main() {
    part2();
}