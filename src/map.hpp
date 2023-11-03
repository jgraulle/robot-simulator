#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>

namespace sf
{
    class Shape;
}


class Map : public sf::Drawable
{
public:
    //! Load a 2d map
    Map(const std::string & mapFilePath, const sf::Color & collisionColor,
            const std::array<float, 3> & lineTrackColorToGreyWeight = {0.2126, 0.7152, 0.0722});

    inline sf::Vector2u getSize() const {return _texture.getSize();}

    //! @return true if outside of the map or color of this coordonate is collisionColor
    inline bool getIsCollision(std::size_t x, std::size_t y) const
            {return x<0.0 || x>=getSize().x || y<0.0 || y>=getSize().y || _collisionTable[x][y];}

    //! @return true if outside of the map or color of this coordonate is collisionColor
    inline std::uint8_t getLineValue(std::size_t x, std::size_t y) const {
        if (x<0.0 || x>=getSize().x || y<0.0 || y>=getSize().y) return 0u;
        return _lineTable[x][y];
    }

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    template <typename Value>
    using Table2d = std::vector<std::vector<Value>>;
    template <typename Value>
    Table2d<Value> prepareTableHelper(const sf::Image & image,
            std::function<Value(const sf::Color &)> convert);

    sf::Texture _texture;
    Table2d<bool> _collisionTable;
    Table2d<std::uint8_t> _lineTable;
    std::unique_ptr<sf::Shape> _shape;
};

#endif