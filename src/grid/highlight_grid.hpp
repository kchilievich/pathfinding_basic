#pragma once

#include "grid.hpp"

namespace pf
{
    class HighlightGrid : public Grid
    {
        public:
        HighlightGrid(float cell_size, sf::RenderWindow* main_window) : Grid(cell_size)
        {
            main_window_ = main_window;
        }

        void Tick(float delta_time)
        {
            if (main_window_)
            {
                float grid_cell_size = get_cell_size();
                sf::Vector2i cursor_local_position = sf::Mouse::getPosition(*main_window_);

                selected_cell_.x = std::floor(cursor_local_position.x / grid_cell_size);
                selected_cell_.y = std::floor(cursor_local_position.y / grid_cell_size);
            }
        }

        protected:
 
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            { // Cell highlight
                float grid_cell_size = get_cell_size();

                float highlight_position_x = grid_cell_size * selected_cell_.x;
                float highlight_position_y = grid_cell_size * selected_cell_.y;

                sf::RectangleShape cell_highlight({grid_cell_size, grid_cell_size});
                cell_highlight.setPosition({highlight_position_x, highlight_position_y});
                cell_highlight.setFillColor(sf::Color(123, 123, 123, 100));
                target.draw(cell_highlight);
            }

            Grid::draw(target, states);
        }

        private:

        pf::Cell selected_cell_;

        // TODO: Consider making that an interface with smart pointers and shit
        sf::RenderWindow* main_window_;
    };
}