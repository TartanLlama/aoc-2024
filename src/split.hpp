#pragma once

#include <ranges>
#include <sstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <tl/to.hpp>
#include <string_view>
#include <array>

namespace tl {

    template <class T, std::size_t N>
    auto split_impl(std::string const& s, std::string_view delim, std::integral_constant<std::size_t, N>) {
        std::array<T, N> ret;
        auto split_range = s | std::views::split(delim)
            | std::views::transform([](auto s) {
            std::stringstream ss;
            ss.str(std::string(s.begin(), s.end()));
            T t;
            ss >> t;
            return t;
                });
        for (auto [i, e] : std::views::enumerate(split_range)) {
            ret[i] = e;
        }
        return ret;
    }

    template <class T>
    auto split_impl(std::string const& s, std::string_view delim, std::integral_constant<std::size_t, 2>) {
        auto from_substr = [&s](std::size_t from, std::size_t to) {
            std::stringstream ss;
            ss.str(s.substr(from, to));
            T t;
            ss >> t;
            return t;
            };

        auto pos = s.find(delim);
        return std::pair{ from_substr(0, pos), from_substr(pos + delim.size(), std::string::npos)};
    }

    template <class T, std::size_t N=0>
    struct split_func {
        auto operator()(std::string const& s, std::string_view delim) const {
            return split_impl<T>(s, delim, std::integral_constant<std::size_t, N>{});
        }
    };

    template <class T>
    struct split_func<T,0> {
        auto operator()(std::string const& s, std::string_view delim) const {
            return s | std::views::split(delim)
                | std::views::transform([](auto s) {
                std::stringstream ss;
                ss.str(std::string(s.begin(), s.end()));
                T t;
                ss >> t;
                return t;
                    }) | tl::to<std::vector>();
        }

        auto operator()(std::istream& stream, std::string_view delim) const {
            std::string s(std::istreambuf_iterator<char>(stream), {});
            return split<T>(s, delim);
        }

        auto operator()(std::string const& s, char delim) const {
            return split<T>(s, std::string_view(&delim, 1));
        }

        auto operator()(std::istream& stream, char delim) const {
            return split<T>(stream, std::string_view(&delim, 1));
        }
    };

    template <class T, std::size_t N=0>
    constexpr auto inline split = split_func<T, N>{};
}