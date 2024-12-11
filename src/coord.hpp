#pragma once

#include <cstdint>
#include <array>
#include <generator>
#include <ranges>
#include <tl/cartesian_product.hpp>

namespace tl {
    class coord {
    public:
        constexpr coord() = default;
        constexpr coord(int x, int y) : x_(x), y_(y) {}
        constexpr coord(coord const&) = default;
        constexpr coord& operator=(coord const&) = default;

        auto x() const { return x_; }
        auto y() const { return y_; }

        friend auto operator<=>(coord const&, coord const&) = default;
        friend auto operator+(coord const& lhs, coord const& rhs) {
            return coord{ lhs.x_ + rhs.x_, lhs.y_ + rhs.y_ };
        }
        friend auto operator-(coord const& lhs, coord const& rhs) {
            return coord{ lhs.x_ - rhs.x_, lhs.y_ - rhs.y_ };
        }
        friend auto operator+(coord const& lhs, int rhs) {
            return coord{ lhs.x_ + rhs, lhs.y_ + rhs };
        }
        friend auto operator-(coord const& lhs, int rhs) {
            return coord{ lhs.x_ - rhs, lhs.y_ - rhs };
        }
        friend auto operator*(coord const& lhs, int rhs) {
            return coord{ lhs.x_ * rhs, lhs.y_ * rhs };
        }
        friend auto operator/(coord const& lhs, int rhs) {
            return coord{ lhs.x_ / rhs, lhs.y_ / rhs };
        }
        coord& operator+=(coord const& rhs) {
            x_ += rhs.x_;
            y_ += rhs.y_;
            return *this;
        }
        coord& operator+=(int rhs) {
            x_ += rhs;
            y_ += rhs;
            return *this;
        }
        coord& operator-=(coord const& rhs) {
            x_ -= rhs.x_;
            y_ -= rhs.y_;
            return *this;
        }
        coord& operator-=(int rhs) {
            x_ -= rhs;
            y_ -= rhs;
            return *this;
        }

        bool in_bounds(int height, int width) const {
            return x_ >= 0 and y_ >= 0 and x_ < height and y_ < width;
        }

        std::array<std::size_t, 2> index() const {
            return { static_cast<std::size_t>(x_), static_cast<std::size_t>(y_) };
        }

        static auto all_coords(int height, int width) {
            return tl::views::cartesian_product(
                std::views::iota(0, height),
                std::views::iota(0, width)
            ) | std::views::transform([](auto&& p) {
                return coord{ p.first, p.second };
            });
        }

        coord up() const {
            return *this - coord(1, 0);
        }
        coord down() const {
            return *this + coord(1, 0);
        }
        coord left() const {
            return *this - coord(0, 1);
        }
        coord right() const {
            return *this + coord(0, 1);
        }
        coord up_left() const {
            return *this - coord(1, 1);
        }
        coord up_right() const {
            return *this - coord(1, -1);
        }
        coord down_left() const {
            return *this + coord(1, -1);
        }
        coord down_right() const {
            return *this + coord(1, 1);
        }
        coord& turn_right();
        coord& turn_left();

    private:
        int x_ = 0;
        int y_ = 0;
    };

    namespace directions {
        extern std::array<coord, 4> const orthogonals;
        extern std::array<coord, 4> const diagonals;
        extern std::array<coord, 8> const all;
        inline constexpr coord up{ -1, 0 };
        inline constexpr coord down{ 1, 0 };
        inline constexpr coord left{ 0, -1 };
        inline constexpr coord right{ 0, 1 };
    }

    inline coord& coord::turn_right() {
        if (*this == directions::up) {
            *this = directions::right;
        }
        else if (*this == directions::right) {
            *this = directions::down;
        }
        else if (*this == directions::down) {
            *this = directions::left;
        }
        else {
            *this = directions::up;
        }
        return *this;
    }
    inline coord& coord::turn_left() {
        if (*this == directions::up) {
            *this = directions::left;
        }
        else if (*this == directions::left) {
            *this = directions::down;
        }
        else if (*this == directions::down) {
            *this = directions::right;
        }
        else {
            *this = directions::up;
        }
        return *this;
    }
}

namespace std {
    template <>
    struct hash<tl::coord> {
        std::size_t operator()(tl::coord const& c) const {
            return std::hash<int>{}(c.x()) ^ std::hash<int>{}(c.y());
        }
    };
}