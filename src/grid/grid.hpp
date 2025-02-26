#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include "cell.hpp"

namespace pf 
{
    class Grid : public sf::Drawable
    {
    public:
        Grid(float cell_size, sf::RenderWindow* main_window);

        float GetCellSize() const { return cell_size; }
        int GetLineLength() const { return line_length; }
        int GetColumnHeight() const { return column_height; }
        
        /// @brief Returns a pointer to a Cell, that covers given position on screen
        /// @param x X position
        /// @param y Y position
        /// @return a Cell pointer if a cell exists at given position, nullptr otherwise
        Cell* GetCellAtPosition(float x, float y);

        /// @brief Returns a pointer to a Cell, that is located at given coordinates
        /// @param coord_x X coordinate
        /// @param coord_y Y coordinate
        /// @return a Cell pointer if a cell with given coordinates exists, nullptr otherwise
        Cell* GetCellAtCoordinates(int coord_x, int coord_y);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        const sf::RenderWindow* GetMainWindow() const { return main_window; }

    private:
        std::vector<Cell> cells;
        float cell_size = 0.f;
        
        int line_length = 0;
        int column_height = 0;

        // TODO: Consider making that an interface with smart pointers and shit
        sf::RenderWindow* main_window;
    };
}