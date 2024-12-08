#include <tl/getlines.hpp>
#include <tl/adjacent.hpp>
#include <tl/functional/curry.hpp>
#include <unordered_map>
#include <ranges>
#include <unordered_set>
#include <fstream>
#include "../split.hpp"
#include "../lift.hpp"
#include <tl/fold.hpp>

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day05/input.txt" };

    std::unordered_map<int, std::unordered_set<int>> orderings;
    for (auto line : tl::views::getlines(input)) {
        if (line.empty()) {
            break;
        }
        auto [a, b] = tl::split<int, 2>(line, "|");
        orderings[a].insert(b);
    }
    auto ordering_func = [&](int a, int b) { return orderings[a].contains(b); };

    auto correct_page_middles = tl::views::getlines(input)
        | std::views::transform(std::bind_back(tl::split<int>, ','))
        | std::views::filter(std::bind_back(std::ranges::is_sorted, ordering_func))
        | std::views::transform([](auto&& pages) {
            return pages[pages.size() / 2];
          });

    std::cout << tl::sum(correct_page_middles);
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day05/input.txt" };

    std::unordered_map<int, std::unordered_set<int>> orderings;
    for (auto line : tl::views::getlines(input)) {
        if (line.empty()) {
            break;
        }
        auto [a, b] = tl::split<int, 2>(line, "|");
        orderings[a].insert(b);
    }
    auto ordering_func = [&](int a, int b) { return orderings[a].contains(b); };
    auto midpoint = [](auto&& r) { return begin(r) + (r.size() / 2); };
    auto corrected_page_middles = tl::views::getlines(input)
        | std::views::transform(std::bind_back(tl::split<int>, ','))
        | std::views::filter([&](auto&& pages) {
            return !std::ranges::is_sorted(pages, ordering_func);
          })
        | std::views::transform([&](auto&& pages) {
            std::ranges::nth_element(pages, midpoint(pages), ordering_func);
            return *midpoint(pages);
          });

    std::cout << tl::sum(corrected_page_middles);
}

int main() {
    part1();
}