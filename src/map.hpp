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
    Map(const std::string & mapFilePath);

    inline bool get(std::size_t x, std::size_t y) const {return _table[x][y];}
    inline sf::Vector2u getSize() const {return _texture.getSize();}

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
    using Bool2dTable = std::vector<std::vector<bool>>;

    sf::Texture _texture;
    Bool2dTable _table;
    std::unique_ptr<sf::Shape> _shape;
};

#endif