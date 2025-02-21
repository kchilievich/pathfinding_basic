#include "grid.hpp"

pf::Grid::Grid(float cell_size, sf::RenderWindow* main_window)
{
    cell_size_ = cell_size;
    main_window_ = main_window;

    auto [width, height] = main_window->getSize();

    line_length_ = std::floor(width / cell_size);
    column_height_ = std::floor(height / cell_size);

    for (int i = 0; i < line_length_; ++i)
        for (int j = 0; j < column_height_; ++j)
        {
            Cell cell{i,j};
            cell.BuildDrawable(cell_size);
            cells_.push_back(std::move(cell));
        }
}

pf::Cell* pf::Grid::GetCellAtPosition(float x, float y)
{
    int coord_x = std::floor(x / cell_size_);
    int coord_y = std::floor(y / cell_size_);

    return GetCellAtCoordinates(coord_x, coord_y);
}

pf::Cell* pf::Grid::GetCellAtCoordinates(int coord_x, int coord_y)
{
    size_t index = coord_x * column_height_ + coord_y;

    if (cells_.size() <= index)
    {
        return nullptr;
    }

    return &cells_[index];
}

void pf::Grid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (cell_size_ <= 0)
    {
        return;
    }

    for (const Cell& cell : cells_)
    {
        target.draw(cell.GetShape());
    }
}
