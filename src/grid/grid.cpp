#include "grid.hpp"

pf::Grid::Grid(float cell_size, sf::RenderWindow* main_window)
{
    cell_size = cell_size;
    main_window = main_window;

    auto [width, height] = main_window->getSize();

    line_length = std::floor(width / cell_size);
    column_height = std::floor(height / cell_size);

    for (int i = 0; i < line_length; ++i)
        for (int j = 0; j < column_height; ++j)
        {
            Cell cell{i,j};
            cell.BuildDrawable(cell_size);
            cells.push_back(std::move(cell));
        }
}

pf::Cell* pf::Grid::GetCellAtPosition(float x, float y)
{
    int coord_x = std::floor(x / cell_size);
    int coord_y = std::floor(y / cell_size);

    return GetCellAtCoordinates(coord_x, coord_y);
}

pf::Cell* pf::Grid::GetCellAtCoordinates(int coord_x, int coord_y)
{
    size_t index = coord_x * column_height + coord_y;

    if (cells.size() <= index)
    {
        return nullptr;
    }

    return &cells[index];
}

void pf::Grid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (cell_size <= 0)
    {
        return;
    }

    for (const Cell& cell : cells)
    {
        target.draw(cell.GetShape());
    }
}
