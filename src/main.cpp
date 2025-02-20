#include <SFML/Graphics.hpp>

#include "grid/grid.hpp"
#include <initializer_list>
#include <vector>

// TODO: Temp global array, move to singleton
static std::vector<sf::Drawable*> g_drawables;

template<class T = sf::Drawable, typename... Args>
static T* NewDrawable(Args... args)
{
    T* new_instance = new T(args...);
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

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Pathfinding Study");
    window.setFramerateLimit(144);

    pf::Grid* main_grid = NewDrawable<pf::Grid>(30.f);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();

        // TODO: Move into a separate drawable
        { // Cell highlight
            float grid_cell_size = main_grid->get_cell_size();
            sf::Vector2i cursor_local_position = sf::Mouse::getPosition(window);

            float x = grid_cell_size * std::floor(cursor_local_position.x / grid_cell_size);
            float y = grid_cell_size * std::floor(cursor_local_position.y / grid_cell_size);

            sf::RectangleShape cell_highlight({grid_cell_size, grid_cell_size});
            cell_highlight.setPosition({x, y});
            cell_highlight.setFillColor(sf::Color(123, 123, 123, 78));
            window.draw(cell_highlight);
        }
 
        DrawDrawables(window);

        window.display();
    }
}
