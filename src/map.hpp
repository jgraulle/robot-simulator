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
    //! Load a 2d map
    Map(const std::string & mapFilePath, const sf::Color & collisionColor,
            const sf::Color & lineColor);

    inline sf::Vector2u getSize() const {return _texture.getSize();}

    //! @return true if outside of the map or color of this coordonate is collisionColor
    inline bool getIsCollision(std::size_t x, std::size_t y) const
            {return x<0.0 || x>=getSize().x || y<0.0 || y>=getSize().y || _collisionTable[x][y];}

    //! @return true if outside of the map or color of this coordonate is collisionColor
    inline bool getIsLine(std::size_t x, std::size_t y) const
            {return x<0.0 || x>=getSize().x || y<0.0 || y>=getSize().y || _lineTable[x][y];}

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    using Bool2dTable = std::vector<std::vector<bool>>;

    Bool2dTable prepareTableHelper(const sf::Image & image, const sf::Color & color);

    sf::Texture _texture;
    Bool2dTable _collisionTable;
    Bool2dTable _lineTable;
    std::unique_ptr<sf::Shape> _shape;
};

#endif