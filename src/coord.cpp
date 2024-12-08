#include "coord.hpp"

namespace tl {
    std::array<coord, 4> const directions::orthogonals = { coord(0, 1), coord(1, 0), coord(0, -1), coord(-1, 0) };
    std::array<coord, 4> const directions::diagonals = { coord(1, 1), coord(1, -1), coord(-1, -1), coord(-1, 1) };
    std::array<coord, 8> const directions::all = { coord(0, 1), coord(1, 1), coord(1, 0), coord(1, -1), coord(0, -1), coord(-1, -1), coord(-1, 0), coord(-1, 1) };
}