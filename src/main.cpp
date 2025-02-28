#include <SFML/Graphics.hpp>
#include <initializer_list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include "grid/grid.hpp"
#include "pathfinding/astar.hpp"

// TODO: Temp global array, move to singleton
static std::vector<sf::Drawable*> g_drawables;

template<class T = sf::Drawable, typename... Args>
static T* NewDrawable(Args&&... args)
{
    T* new_instance = new T(std::forward<Args>(args)...);
    g_drawables.push_back(new_instance);

    return new_instance;
}

static void DrawDrawables(sf::RenderTarget* target)
{
    for (sf::Drawable* drawable : g_drawables)
    {
        target->draw(*drawable);
    }
}
// TODO END

pf::Grid* InitMainGrid(std::shared_ptr<sf::RenderWindow> window)
{
    pf::Grid* grid_ptr;
    std::ifstream map("map.json");
    
    if (map.fail())
    {
        grid_ptr = NewDrawable<pf::Grid>(20.f, window);
    }
    else
    {
        grid_ptr = pf::Grid::Deserialize(map, window);
        g_drawables.push_back(grid_ptr);
    }

    map.close();
    return grid_ptr;
}

int main()
{
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Pathfinding Study"));
    window->setFramerateLimit(144);

    pf::Grid* main_grid = InitMainGrid(window);

    pf::Cell* start = main_grid->GetCellAtCoordinates(1, 1);
    pf::Cell* target = main_grid->GetCellAtCoordinates(80, 40);

    bool selecting_start = true;

    std::vector<sf::CircleShape*> tmp_drawable_path;

    std::chrono::high_resolution_clock::time_point last_frame;

    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                std::ofstream fmap("map.json", std::ios::out);
                main_grid->Serialize(fmap);

                fmap.close();
                window->close();
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
                    const std::vector<const pf::Cell*>& path = pf::AStar::AStar(main_grid, start, target);
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

        window->clear();
 
        DrawDrawables(window.get());

        window->display();
    }

    g_drawables.clear();
}
