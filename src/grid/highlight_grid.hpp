#pragma once

#include <sstream>
#include "grid.hpp"

namespace pf
{
    class HighlightGrid : public Grid
    {
        public:

        HighlightGrid(float cell_size, sf::RenderWindow* main_window) : Grid(cell_size, main_window) 
        {
            coordinates_string_font = sf::Font("C:\\Windows\\Fonts\\Arial.ttf");
            coordinates_string = new sf::Text(coordinates_string_font);
            coordinates_string->setPosition({30.f, 30.f});
            coordinates_string->setCharacterSize(24);
            coordinates_string->setOutlineColor(sf::Color(70, 70, 70, 240));
            coordinates_string->setFillColor(sf::Color(180, 180, 180, 180));
            coordinates_string->setOutlineThickness(0.5f);
        }

        void Tick(float delta_time)
        {
            SelectCell();

            if (!selected_cell)
            {
                return;
            }
            
            std::stringstream ss;
            ss << "Selected Cell: X: " << selected_cell->x << " Y: " << selected_cell->y << std::endl;

            auto str = sf::String(ss.str());
            coordinates_string->setString(str.getData());
        }

        void MarkSelectedCellAsBlocked()
        {
            if (!selected_cell)
            {
                return;
            }

            selected_cell->ToggleBlocked();
        }

        pf::Cell* GetSelectedCell() const
        {
            return selected_cell;
        }

        private:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            Grid::draw(target, states);
            
            coordinates_string->setFont(coordinates_string_font);
            target.draw(*coordinates_string, states);
        }

        void SelectCell()
        {
            const sf::RenderWindow* window = GetMainWindow();
            if (!window)
            {
                return;
            }

            float grid_cell_size = GetCellSize();
            sf::Vector2i cursor_local_position = sf::Mouse::getPosition(*window);

            pf::Cell* new_selected_cell = GetCellAtPosition((float)cursor_local_position.x, (float)cursor_local_position.y);
            if (!new_selected_cell)
            {
                return;
            }

            if (new_selected_cell == selected_cell)
            {
                return;
            }

            if (selected_cell)
            {
                selected_cell->ToggleHighlighted(false);
            }
            selected_cell = new_selected_cell;
            selected_cell->ToggleHighlighted(true);
        }

        pf::Cell* selected_cell = nullptr;

        sf::Text* coordinates_string = nullptr;
        sf::Font coordinates_string_font;
    };
}