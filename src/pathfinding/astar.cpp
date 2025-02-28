#include "astar.hpp"

float pf::AStar::EuclidianDistance(const pf::Cell* node, const pf::Cell* target)
{
    return std::sqrt(std::pow(node->x - target->x, 2) + std::pow(node->y - target->y, 2));
}

std::vector<const pf::Cell* > pf::AStar::ReconstructPath(std::map<const pf::Cell*, const pf::Cell*> came_from, const pf::Cell* current)
{
    std::vector<const pf::Cell*> path {current};
    while(came_from[current] != nullptr)
    {
        current = came_from[current];
        path.insert(path.begin(), current);
    }

    return path;
}

std::vector<const pf::Cell*> pf::AStar::AStar(pf::Grid* grid, const pf::Cell* start, const pf::Cell* target)
{
    std::vector<const pf::Cell*> open_set{start};
    
    // For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from the start
    // to n currently known.
    std::map<const pf::Cell*, const pf::Cell*> came_from;

    // For node n, gScore[n] is the currently known cost of the cheapest path from start to n.
    std::map<const pf::Cell*, float> g_score {{start, 0.f}};

    // For node n, fScore[n] := gScore[n] + h(n). fScore[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    std::map<const pf::Cell*, float> f_score {{start, EuclidianDistance(start, target)}};

    while (open_set.size())
    {
        const pf::Cell* current_cell;
        size_t current_cell_index = -1;

        { // Find the cell with lowest f_score
            float lowest_score = FLT_MAX;
            for(size_t i = 0; i < open_set.size(); ++i)
            {
                const pf::Cell* cell = open_set[i];
                const float cell_score = f_score[cell];
                if (cell_score < lowest_score)
                {
                    current_cell = cell;
                    lowest_score = cell_score;
                    current_cell_index = i;
                }
            }
        }

        if (current_cell == target)
        {
            return ReconstructPath(came_from, current_cell);
        }

        open_set.erase(open_set.begin() + current_cell_index);

        // Check neighbour cell
        auto check_neighbour = [&](pf::Cell* neighbour) {
            float tentative_g_score = g_score[current_cell] + /*TODO: distance_to(neighboor)*/ 1.f;
            if (g_score.find(neighbour) == g_score.end() || tentative_g_score < g_score[neighbour])
            {
                came_from[neighbour] = current_cell;
                g_score[neighbour] = tentative_g_score;
                f_score[neighbour] = tentative_g_score + EuclidianDistance(neighbour, target);

                if (std::find(open_set.begin(), open_set.end(), neighbour) == open_set.end())
                {
                    open_set.push_back(neighbour);
                }
            }
        };

        int x = current_cell->x;
        int y = current_cell->y;
       
        std::vector<std::pair<int, int>> offsets {
            {1, 0},
            {1, 1},
            {0, 1},
            {-1, 0},
            {-1, -1},
            {0, -1},
            {1, -1},
            {-1, 1}
       };
       for (std::pair<int, int> offset : offsets)
       {
            pf::Cell* neighbour_cell = grid->GetCellAtCoordinates(x + offset.first, y + offset.second);
            if (!neighbour_cell || neighbour_cell->IsBlocked())
            {
                continue;
            }
            check_neighbour(neighbour_cell);
       }
    }

    return {};
}
