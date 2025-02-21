#pragma once

#include "grid.hpp"

namespace pf
{
    class HighlightGrid : public Grid
    {
        public:

        HighlightGrid(float cell_size, sf::RenderWindow* main_window) : Grid(cell_size, main_window) {}

        void Tick(float delta_time)
        {
            SelectCell();
        }

        private:

        void SelectCell()
        {
            const sf::RenderWindow* window = GetMainWindow();
            if (!window)
            {
                return;
            }

            float grid_cell_size = GetCellSize();
            sf::Vector2i cursor_local_position = sf::Mouse::getPosition(*window);

            pf::Cell* new_selected_cell = GetCellAtPosition(cursor_local_position.x, cursor_local_position.y);
            if (!new_selected_cell)
            {
                return;
            }

            if (new_selected_cell == selected_cell_)
            {
                return;
            }

            if (selected_cell_)
            {
                selected_cell_->SetFillColor(pf::Cell::GetDefaultFillColor());
            }
            selected_cell_ = new_selected_cell;
            selected_cell_->SetFillColor({123, 123, 123, 100});
        }

        pf::Cell* selected_cell_ = nullptr;
    };
}