#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace pf 
{
    struct Cell
    {
        int x;
        int y;
    };

    class Grid : public sf::Drawable
    {
    public:
        Grid(float cell_size)
        {
            cell_size_ = cell_size;
        }

        float get_cell_size() const { return cell_size_; }

        protected:

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            if (cell_size_ <= 0)
            {
                return;
            }

            // TODO: Fix resize
            auto [width, height] = target.getSize();

            float pos = 1.f;
            while (pos <= width || pos <= height)
            {
                if (pos <= width)
                {
                    std::array line =
                        {
                            sf::Vertex{sf::Vector2f(pos, 0.f)},
                            sf::Vertex{sf::Vector2f(pos, (float)height)}
                        };

                    target.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
                }
                if (pos <= height)
                {
                    std::array line =
                       {
                           sf::Vertex{sf::Vector2f(0.f, pos)},
                           sf::Vertex{sf::Vector2f((float)width, pos)}
                       };
                    
                    target.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
                }

                pos += cell_size_;
            }
        };

        private:
        float cell_size_ = 0.f;
    };
}