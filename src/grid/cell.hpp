#pragma once

#include <SFML/Graphics.hpp>

namespace pf 
{
    struct Cell
    {
        Cell() : x(0), y(0) {}
        Cell(int in_x, int in_y) : x(in_x), y(in_y) {}

        sf::Vector2f GetPosition(float cell_size) const
        {
            return {x * cell_size, y * cell_size};
        }

        void BuildDrawable(float cell_size)
        {
            shape.setSize({cell_size, cell_size});
            shape.setPosition(GetPosition(cell_size));
            shape.setFillColor(fill_color);
            shape.setOutlineThickness(1.f);
            shape.setOutlineColor(outline_color);
        }

        const sf::RectangleShape GetShape() const 
        { 
            return shape; 
        }

        void ToggleBlocked()
        {
            is_blocked = !is_blocked;
            UpdateFillColor();
        }

        void ToggleBlocked(bool new_blocked)
        {
            is_blocked = new_blocked;
            UpdateFillColor();
        }

        void ToggleHighlighted(bool new_highlighted)
        {
            is_highlighted = new_highlighted;
            UpdateFillColor();
        }

        int x;
        int y;
    private:
        void UpdateFillColor()
        {
            // TODO: Maybe it's time to play with shaders for a bit here, but masks work for now
            sf::Color resulting_color = fill_color;
            if (is_blocked)
            {
                resulting_color += blocked_mask;
            }
            if (is_highlighted)
            {
                resulting_color += highlighted_mask;
            }

            shape.setFillColor(resulting_color);
        }
        
        void SetFillColor(sf::Color new_color)
        {
            fill_color = new_color;
            UpdateFillColor();
        }

        void SetOutlineColor(sf::Color new_color)
        {
            outline_color = new_color;
            UpdateFillColor();
        }

        // Logical
        bool is_blocked = false;
        bool is_highlighted = false;

        // Visual
        sf::Color fill_color = sf::Color::Transparent;
        sf::Color outline_color = sf::Color::White;
        sf::Color blocked_mask = sf::Color::Red;
        sf::Color highlighted_mask = sf::Color(150, 150, 150, 100);

        sf::RectangleShape shape;
    };
}