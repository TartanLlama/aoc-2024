#pragma once

#include "coord.hpp"
#include <generator>
#include <fstream>
#include <vector>
#include <string>
#include <ranges>
#include <functional>
#include <iostream>
#include <tl/cartesian_product.hpp>

namespace tl {
    template <auto V>
    struct mark_out_of_bounds_with {
        static inline auto value = V;
        static constexpr inline bool mark_oob = true;
        static auto& oob() {
            return value;
        }
    };
    struct out_of_bounds_is_ub {
        static constexpr inline bool mark_oob = false;
    };
    struct out_of_bounds_is_error {
        static constexpr inline bool mark_oob = true;
        static auto oob() {
            throw std::out_of_range("Out of bounds access");
        }
    };

    template <class T, class OOBPolicy = out_of_bounds_is_ub>
    class grid {
    public:
        grid(std::size_t height, std::size_t width) : height_(height), width_(width), data_(height* width) {}

        template <class Rng>
        requires std::is_same_v<T,char>
        grid(std::size_t width, Rng&& rng) : width_(width), data_(std::begin(rng), std::end(rng)) {
            height_ = data_.size() / width_;
        }

        template <class Rng>
            requires std::integral<T> and !std::is_same_v<T, char>
        grid(std::size_t width, Rng&& rng) : width_(width) {
            height_ = std::ranges::distance(rng) / width_;
            data_.reserve(std::ranges::distance(rng));
            for (auto&& e : rng) {
                data_.push_back(e - '0');
            }
        }

        grid(std::ifstream& input) {
            std::string text;
            std::getline(input, text, '\0');
            auto width = text.find('\n');
            *this = grid{ width, text | std::views::filter([](auto c) { return c != '\n'; }) };
        }

        T& operator[](std::size_t i, std::size_t j) {
            if constexpr (OOBPolicy::mark_oob) {
                if (!in_bounds(coord(i, j))) {
                    return OOBPolicy::oob();
                }
            }
            return data_[i * width_ + j];
        }
        T const& operator[](std::size_t i, std::size_t j) const {
            if constexpr (OOBPolicy::mark_oob) {
                if (!in_bounds(coord(i, j))) {
                    return OOBPolicy::oob();
                }
            }
            return data_[i * width_ + j];
        }
        T& operator[](coord c) {
            if constexpr (OOBPolicy::mark_oob) {
                if (!in_bounds(c)) {
                    return OOBPolicy::oob();
                }
            }
            return data_[c.x() * width_ + c.y()];
        }
        T const& operator[](coord c) const {
            if constexpr (OOBPolicy::mark_oob) {
                if (!in_bounds(c)) {
                    return OOBPolicy::oob();
                }
            }
            return data_[c.x() * width_ + c.y()];
        }
        std::size_t height() const {
            return height_;
        }
        std::size_t width() const {
            return width_;
        }
        bool in_bounds(coord c) const {
            return c.x() >= 0 and c.y() >= 0 and c.x() < height_ and c.y() < width_;
        }

        void dump() const {
            for (auto i = 0; i < height_; ++i) {
                for (auto j = 0; j < width_; ++j) {
                    std::cout << (*this)[i,j];
                }
                std::cout << '\n';
            }
            std::cout << '\n';
        }

        auto indexer() const {
            return [this](coord c) -> T& {
                return (*this)[c];
            };
        }

        auto all_coords() const {
            return tl::views::cartesian_product(std::views::iota(0u, height_), std::views::iota(0u, width_)) 
                | std::views::transform([](auto&& p) { return coord(p.first, p.second); });
        }

        std::generator<coord> coords_within_border(std::size_t border) const {
            for (auto i = border; i < height_ - border; ++i) {
                for (auto j = border; j < width_ - border; ++j) {
                    co_yield coord(i, j);
                }
            }
        }

        template <class Dirs>
        auto slices_in_directions(const Dirs& directions, coord from) {
            auto walker = [&](tl::coord from, tl::coord mod) -> std::generator<char> {
                while (in_bounds(from)) {
                    co_yield (*this)[from];
                    from += mod;
                }
            };
            return directions | std::views::transform(std::bind_front(walker, from));
        };

    private:
        std::size_t height_;
        std::size_t width_;
        std::vector<T> data_;
    };
}