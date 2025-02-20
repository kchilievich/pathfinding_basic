#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace pf 
{
    class Grid : public sf::Drawable
    {
    public:
        Grid(float cell_size)
        {
            cell_size_ = cell_size;
        }

        float get_cell_size() const { return cell_size_; }

        protected:

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            if (cell_size_ <= 0)
            {
                return;
            }

            // TODO: Fix resize
            auto [width, height] = target.getSize();

            // TODO: Merge into one cycle
            float x_pos = 0;
            while (x_pos < width)
            {
                std::array line =
                    {
                        sf::Vertex{sf::Vector2f(x_pos, 0.f)},
                        sf::Vertex{sf::Vector2f(x_pos, (float)height)}
                    };

                target.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
                x_pos += cell_size_;
            }
            
            float y_pos = 0;
            while (y_pos < width)
            {
                std::array line =
                    {
                        sf::Vertex{sf::Vector2f(0.f, y_pos)},
                        sf::Vertex{sf::Vector2f((float)width, y_pos)}
                    };

                target.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
                y_pos += cell_size_;
            }
        };

        private:
        float cell_size_ = 0.f;
    };
}