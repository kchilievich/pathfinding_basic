#pragma once

#include <vector>
#include <map>

#include "grid/cell.hpp"
#include "grid/grid.hpp"

namespace pf::AStar
{
    float EuclidianDistance(const pf::Cell* node, const pf::Cell* target);

    std::vector<const pf::Cell*> ReconstructPath(std::map<const pf::Cell*, const pf::Cell*> came_from, const pf::Cell* current);

    std::vector<const pf::Cell*> AStar(pf::Grid* grid, const pf::Cell* start, const pf::Cell* target);
}