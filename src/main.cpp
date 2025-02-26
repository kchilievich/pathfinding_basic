#include <SFML/Graphics.hpp>

#include "grid/highlight_grid.hpp"
#include <initializer_list>
#include <vector>
#include <map>
#include <iostream>

// TODO: Temp global array, move to singleton
static std::vector<sf::Drawable*> g_drawables;

template<class T = sf::Drawable, typename... Args>
static T* NewDrawable(Args&&... args)
{
    T* new_instance = new T(std::forward<Args>(args)...);
    g_drawables.push_back(new_instance);

    return new_instance;
}

static void DrawDrawables(sf::RenderTarget& target)
{
    for (sf::Drawable* drawable : g_drawables)
    {
        target.draw(*drawable);
    }
}
// TODO END

namespace pf::Logic
{
float EuclidianDistance(const pf::Cell* node, const pf::Cell* target)
{
    return std::sqrt(std::pow(node->x - target->x, 2) + std::pow(node->y - target->y, 2));
}

std::vector<const pf::Cell*> ReconstructPath(std::map<const pf::Cell*, const pf::Cell*> came_from, const pf::Cell* current)
{
    std::vector<const pf::Cell*> path {current};
    while(came_from[current] != nullptr)
    {
        current = came_from[current];
        path.insert(path.begin(), current);
    }

    return path;
}

std::vector<const pf::Cell*> AStar(pf::Grid* grid, const pf::Cell* start, const pf::Cell* target)
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
}

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Pathfinding Study");
    window.setFramerateLimit(144);

    pf::HighlightGrid* main_grid = NewDrawable<pf::HighlightGrid>(20.f, &window);

    pf::Cell* start = main_grid->GetCellAtCoordinates(1, 1);
    pf::Cell* target = main_grid->GetCellAtCoordinates(80, 40);
    bool selecting_start = true;

    std::vector<sf::CircleShape*> tmp_drawable_path;

    std::chrono::high_resolution_clock::time_point last_frame;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    auto new_cell = main_grid->GetSelectedCell();
                    if (!new_cell)
                        continue;

                    if (selecting_start)
                    {
                        start = new_cell;
                    }
                    else
                    {
                        target = new_cell;
                    }

                    selecting_start = !selecting_start;
                }
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    main_grid->MarkSelectedCellAsBlocked();
                }

                // TODO: implement decently
                for (auto shape : tmp_drawable_path)
                {
                    g_drawables.erase(std::find(g_drawables.begin(), g_drawables.end(), shape));
                }
                tmp_drawable_path.clear();

                {
                    using namespace std::chrono;
                    auto astar_begin = system_clock::now();
                    const std::vector<const pf::Cell*>& path = pf::Logic::AStar(main_grid, start, target);
                    auto astar_end = system_clock::now();

                    int delta = (time_point_cast<milliseconds>(astar_end) - time_point_cast<milliseconds>(astar_begin)).count();

                    std::cout << "Path built in: " << delta << "ms" << std::endl;

                    for (const pf::Cell* cell : path)
                    {
                        sf::Vector2f cell_center = cell->GetShape().getPosition();
                        sf::CircleShape* dot = NewDrawable<sf::CircleShape>(main_grid->GetCellSize() / 2.f);
                        tmp_drawable_path.push_back(dot);
                        dot->setPosition(cell_center);
                        dot->setFillColor(sf::Color::Green);
                    }
                }
            }
        }
        
        {
            using namespace std::chrono;

            float delta_time = (time_point_cast<milliseconds>(high_resolution_clock::now()) - time_point_cast<milliseconds>(last_frame)).count() / 1000.f;
            // TODO: Abstract Tickable interface and basically the same flow as with Drawable
            {
                main_grid->Tick(delta_time);
            }

            last_frame = high_resolution_clock::now();
        }

        window.clear();
 
        DrawDrawables(window);

        window.display();
    }

    g_drawables.clear();
}
