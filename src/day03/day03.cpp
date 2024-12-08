#include <fstream>
#include <string>
#include <optional>
#include <iostream>

void part1() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day03/input.txt" };
    std::string text;
    std::getline(input, text, '\0');

    auto try_parse_int = [](const std::string& text, std::size_t& pos) {
        auto orig_pos = pos;
        while (pos < text.size() and std::isdigit(text[pos])) {
            ++pos;
        }
        if (pos == orig_pos) {
            return std::optional<int>{};
        }
        return std::optional(std::stoi(text.substr(orig_pos, pos - orig_pos)));
        };
    auto try_parse_mul = [=](const std::string& text, std::size_t& pos) {
        if (text[pos++] != '(') {
            return 0;
        }
        auto l = try_parse_int(text, pos);
        if (!l) {
            return 0;
        };
        if (text[pos++] != ',') {
            return 0;
        }
        auto r = try_parse_int(text, pos);
        if (!r) {
            return 0;
        }
        if (text[pos++] != ')') {
            return 0;
        }
        return l.value() * r.value();
        };

    std::size_t pos = 0;
    auto sum = 0;
    while (pos < text.size()) {
        auto mul_pos = text.find("mul", pos);
        if (mul_pos == std::string::npos) {
            break;
        }
        pos = mul_pos + 3;
        sum += try_parse_mul(text, pos);
    }
    std::cout << sum;
}

void part2() {
    std::ifstream input{ "C:/Users/Tarta/projects/aoc-2024/src/day03/input.txt" };
    std::string text;
    std::getline(input, text, '\0');

    auto try_parse_int = [](const std::string& text, std::size_t& pos) {
        auto orig_pos = pos;
        while (pos < text.size() and std::isdigit(text[pos])) {
            ++pos;
        }
        if (pos == orig_pos) {
            return std::optional<int>{};
        }
        return std::optional(std::stoi(text.substr(orig_pos, pos - orig_pos)));
        };
    auto try_parse_mul = [=](const std::string& text, std::size_t& pos) {
        if (text[pos++] != '(') {
            return 0;
        }
        auto l = try_parse_int(text, pos);
        if (!l) {
            return 0;
        };
        if (text[pos++] != ',') {
            return 0;
        }
        auto r = try_parse_int(text, pos);
        if (!r) {
            return 0;
        }
        if (text[pos++] != ')') {
            return 0;
        }
        return l.value() * r.value();
        };

    std::size_t pos = 0;
    auto sum = 0;
    bool enabled = true;
    while (pos < text.size()) {
        auto start_pos = text.find_first_of("md", pos);
        if (start_pos == std::string::npos) {
            break;
        }
        if (text.substr(start_pos, 3) == "mul" and enabled) {
            pos = start_pos + 3;
            sum += try_parse_mul(text, pos);
        }
        else if (text.substr(start_pos, 4) == "do()") {
            enabled = true;
            pos = start_pos + 4;
        }
        else if (text.substr(start_pos, 7) == "don't()") {
            enabled = false;
            pos = start_pos + 7;
        }
        else {
            pos = start_pos + 1;
        }
    }
    std::cout << sum;
}

int main() {
    part2();
}