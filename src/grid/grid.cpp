#include "grid.hpp"
#include <cJSON.h>
#include <fstream>
#include <sstream>

pf::Grid::Grid(float in_cell_size, std::weak_ptr<sf::RenderWindow> in_main_window)
{
    cell_size = in_cell_size;
    main_window = in_main_window;

    auto [width, height] = GetMainWindow()->getSize();

    line_length = std::floor(width / cell_size);
    column_height = std::floor(height / cell_size);

    for (int i = 0; i < line_length; ++i)
        for (int j = 0; j < column_height; ++j)
        {
            Cell cell{i,j};
            cell.BuildDrawable(cell_size);
            cells.push_back(std::move(cell));
        }

    // NOTE: that's bad for shipping but I don't want to resolve it rn
    coordinates_string_font = sf::Font("C:\\Windows\\Fonts\\Arial.ttf");
    coordinates_string = new sf::Text(coordinates_string_font);
    coordinates_string->setPosition({30.f, 30.f});
    coordinates_string->setCharacterSize(24);
    coordinates_string->setOutlineColor(sf::Color(70, 70, 70, 240));
    coordinates_string->setFillColor(sf::Color(180, 180, 180, 180));
    coordinates_string->setOutlineThickness(0.5f);
}

pf::Grid::~Grid()
{
    delete coordinates_string;
}

void pf::Grid::Tick(float delta_time)
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

void pf::Grid::MarkSelectedCellAsBlocked()
{
    if (!selected_cell)
    {
        return;
    }

    selected_cell->ToggleBlocked();
}

pf::Cell *pf::Grid::GetSelectedCell() const
{
    return selected_cell;
}

pf::Cell* pf::Grid::GetCellAtPosition(float x, float y)
{
    int coord_x = std::floor(x / cell_size);
    int coord_y = std::floor(y / cell_size);

    return GetCellAtCoordinates(coord_x, coord_y);
}

pf::Cell* pf::Grid::GetCellAtCoordinates(int coord_x, int coord_y)
{
    if (coord_x < 0 || coord_y < 0)
    {
        return nullptr;
    }

    size_t index = coord_x * column_height + coord_y;

    if (cells.size() <= index)
    {
        return nullptr;
    }

    return &cells[index];
}

void pf::Grid::Serialize(std::ostream& output)
{
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "cell_size", cell_size);
    auto j_cells = cJSON_AddArrayToObject(json, "cells");

    for (const Cell& cell : cells)
    {
        // TODO: Optimize with CDO to not write whole grid of default cells;
        auto j_cell = cell.ToJSON();
        cJSON_AddItemToArray(j_cells, j_cell);
    }

    output << cJSON_Print(json);
    cJSON_Delete(json);
}

pf::Grid* pf::Grid::Deserialize(std::ifstream& input, std::weak_ptr<sf::RenderWindow> window)
{
    pf::Grid* grid = nullptr;
    std::stringstream buffer;
    buffer << input.rdbuf();
    
    cJSON* json = cJSON_Parse(buffer.str().c_str());
    if (json == nullptr)
    {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != nullptr)
        {
            printf("JSON parsing error: %s\n", error_ptr);
        }
       
        cJSON_Delete(json);
        return grid;
    }

    auto cell_size_item = cJSON_GetObjectItem(json, "cell_size");
    if (!cJSON_IsNumber(cell_size_item))
    {
        cJSON_Delete(json);
        return grid;
    }
    
    const float cell_size = cell_size_item->valueint;
    grid = new pf::Grid(cell_size, window);

    std::vector<pf::Cell> cells {};
    auto cells_item = cJSON_GetObjectItem(json, "cells");
    if (!cJSON_IsArray(cells_item))
    {
        cJSON_Delete(json);
        return grid;
    }

    cJSON* cell_item;
    cJSON_ArrayForEach(cell_item, cells_item)
    {
        cJSON* x_item = cJSON_GetObjectItem(cell_item, "x");
        cJSON* y_item = cJSON_GetObjectItem(cell_item, "y");
        cJSON* is_blocked_item = cJSON_GetObjectItem(cell_item, "is_blocked");
        if (!cJSON_IsNumber(x_item) || !cJSON_IsNumber(y_item) || !cJSON_IsBool(is_blocked_item))
        {
            cJSON_Delete(json);
            return grid;
        }

        pf::Cell cell {x_item->valueint, y_item->valueint};
        cell.BuildDrawable(cell_size);
        cell.ToggleBlocked(is_blocked_item->valueint);
        cells.push_back(std::move(cell));
    }

    grid->cells = std::move(cells);

    cJSON_Delete(json);
    return grid;
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

    target.draw(*coordinates_string, states);
}

void pf::Grid::SelectCell()
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

const sf::RenderWindow *pf::Grid::GetMainWindow() const
{
    if (main_window.expired())
    {
        throw std::bad_weak_ptr();
    }

    return main_window.lock().get();
}
