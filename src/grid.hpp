#pragma once

#include "coord.hpp"
#include <generator>
#include <fstream>
#include <vector>
#include <string>
#include <ranges>
#include <functional>
#include <iostream>

namespace tl {
    template <class T>
    class grid {
    public:
        grid(std::size_t height, std::size_t width) : height_(height), width_(width), data_(height* width) {}

        template <class It>
        grid(std::size_t width, It begin, It end) : width_(width), data_(begin, end) {
            height_ = data_.size() / width_;
        }

        template <class Rng>
        grid(std::size_t width, Rng&& rng) : width_(width), data_(std::begin(rng), std::end(rng)) {
            height_ = data_.size() / width_;
        }

        grid(std::ifstream& input) {
            std::string text;
            std::getline(input, text, '\0');
            auto width = text.find('\n');
            *this = grid{ width, text | std::views::filter([](auto c) { return c != '\n'; }) };
        }

        T& operator[](std::size_t i, std::size_t j) {
            return data_[i * width_ + j];
        }
        T const& operator[](std::size_t i, std::size_t j) const {
            return data_[i * width_ + j];
        }
        T& operator[](coord c) {
            return data_[c.x() * width_ + c.y()];
        }
        T const& operator[](coord c) const {
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

        std::generator<coord> all_coords() const {
            for (auto i = 0; i < height_; ++i) {
                for (auto j = 0; j < width_; ++j) {
                    co_yield coord(i, j);
                }
            }
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