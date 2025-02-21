#include <SFML/Graphics.hpp>

#include "grid/highlight_grid.hpp"
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

void AStar(pf::Grid* grid)
{

}

int main()
{
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Pathfinding Study");
    window.setFramerateLimit(144);

    pf::HighlightGrid* main_grid = NewDrawable<pf::HighlightGrid>(30.f, &window);

    std::chrono::high_resolution_clock::time_point frame_start;

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
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    main_grid->MarkSelectedCellAsBlocked();
                }
            }
        }
        
        float delta_time = (std::chrono::high_resolution_clock::now() - frame_start).count() / 1000000000.f /*Nanoseconds of high_resolution_clock to seconds, precision loss is acceptable I guess*/;
        // TODO: Abstract Tickable interface and basically the same flow as with Drawable
        {
            main_grid->Tick(delta_time);
        }

        frame_start = std::chrono::high_resolution_clock::now();

        window.clear();
 
        DrawDrawables(window);

        window.display();
    }

    g_drawables.clear();
}
