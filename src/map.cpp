#include "map.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


Map::Map(const std::string & mapFilePath)
    : _texture()
    , _table()
    , _shape()
{
    // Load image and texture
    sf::Image image;
    if (!image.loadFromFile(mapFilePath))
        exit(1);
    image.createMaskFromColor(sf::Color::Black);
    if (!_texture.loadFromImage(image))
        exit(1);

    // Prepare shape for drawing
    _shape = std::make_unique<sf::RectangleShape>(
            sf::Vector2f(_texture.getSize().x, _texture.getSize().y));
    _shape->setTexture(&_texture);

    // Prepare table for colission detection
    _table.reserve(image.getSize().x);
    for (auto x = 0u; x<image.getSize().x; x++)
    {
        _table.emplace_back();
        _table[x].reserve(image.getSize().y);
        for (auto y = 0u; y<image.getSize().y; y++)
        {
            _table[x].emplace_back(image.getPixel(x, y).a != 0);
        }
    }
}

void Map::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(*_shape, states);
}
