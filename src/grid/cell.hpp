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

        void SetFillColor(sf::Color new_color)
        {
            fill_color = new_color;
            shape.setFillColor(new_color);
        }

        void SetOutlineColor(sf::Color new_color)
        {
            outline_color = new_color;
            shape.setOutlineColor(new_color);
        }

        static sf::Color GetDefaultFillColor()
        {
            return sf::Color::Transparent;
        }

        private:
        sf::Color fill_color = GetDefaultFillColor();
        sf::Color outline_color = sf::Color::White;
        sf::RectangleShape shape;
        int x;
        int y;
    };
}