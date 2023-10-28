#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <vector>
#include <memory>

namespace sf
{
    class Shape;
}


class Map : public sf::Drawable
{
public:
    //! Load a 2d map all black pixel are set to transparent with no collision
    Map(const std::string & mapFilePath);

    inline sf::Vector2u getSize() const {return _texture.getSize();}

    //! @return true if outside of the map or color of this coordonate is not black
    inline bool getIsCollision(std::size_t x, std::size_t y) const
            {return x<0.0 || x>=getSize().x || y<0.0 || y>=getSize().y || _table[x][y];}

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    using Bool2dTable = std::vector<std::vector<bool>>;

    sf::Texture _texture;
    Bool2dTable _table;
    std::unique_ptr<sf::Shape> _shape;
};

#endif