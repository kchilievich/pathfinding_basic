#pragma once

#include "cell.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace pf 
{
    class Grid : public sf::Drawable
    {
    public:
        Grid(float cell_size, std::weak_ptr<sf::RenderWindow> main_window);

        virtual ~Grid();

        void Tick(float delta_time);

        void MarkSelectedCellAsBlocked();

        pf::Cell* GetSelectedCell() const;

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

        void Serialize(std::ostream& output);
        
        static pf::Grid* Deserialize(std::ifstream& input, std::weak_ptr<sf::RenderWindow> window);

    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void SelectCell();

        const sf::RenderWindow* GetMainWindow() const; 

    private:
        std::vector<Cell> cells;
        float cell_size = 0.f;
        
        int line_length = 0;
        int column_height = 0;

        // TODO: Consider making that an interface
        std::weak_ptr<sf::RenderWindow> main_window;

        pf::Cell* selected_cell = nullptr;

        sf::Text* coordinates_string = nullptr;
        sf::Font coordinates_string_font;
    };
}