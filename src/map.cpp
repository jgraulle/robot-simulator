#include "map.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


Map::Map(const std::string & mapFilePath, const sf::Color & collisionColor,
        const sf::Color & lineColor)
    : _texture()
    , _collisionTable()
    , _lineTable()
    , _shape()
{
    // Load image and texture
    sf::Image image;
    if (!image.loadFromFile(mapFilePath))
        exit(1);
    if (!_texture.loadFromImage(image))
        exit(1);

    // Prepare shape for drawing
    _shape = std::make_unique<sf::RectangleShape>(
            sf::Vector2f(_texture.getSize().x, _texture.getSize().y));
    _shape->setTexture(&_texture);

    // Prepare table for collision detection
    _collisionTable = prepareTableHelper(image, collisionColor);

    // Prepare table for line detection
    _lineTable = prepareTableHelper(image, lineColor);
}

void Map::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(*_shape, states);
}

Map::Bool2dTable Map::prepareTableHelper(const sf::Image & image, const sf::Color & color)
{
    Bool2dTable table;

    table.reserve(image.getSize().x);
    for (auto x = 0u; x<image.getSize().x; x++)
    {
        table.emplace_back();
        table[x].reserve(image.getSize().y);
        for (auto y = 0u; y<image.getSize().y; y++)
        {
            table[x].emplace_back(image.getPixel(x, y) == color);
        }
    }

    return table;
}
