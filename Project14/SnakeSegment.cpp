#include "SnakeSegment.h"
#include <iostream>

// Static texture initialization
sf::Texture SnakeSegment::texture;

SnakeSegment::SnakeSegment() {
    // Default constructor; you could leave this empty or use it for dynamic allocation
}

SnakeSegment::SnakeSegment(int x, int y) {
    position.setPosition(x, y);
    sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

    // Load the shared texture only once
    if (texture.getSize().x == 0) {
        if (!texture.loadFromFile("new_snake.png")) {
            std::cerr << "Error loading snake segment texture!" << std::endl;
        }
    }

    sprite.setTexture(texture);

    // Scale the sprite to fit the cell size (40x40)
    float scaleX = 40.0f / texture.getSize().x;
    float scaleY = 40.0f / texture.getSize().y;
    sprite.setScale(scaleX, scaleY);
}

void SnakeSegment::setPosition(int x, int y) {
    position.setPosition(x, y);
    sprite.setPosition(static_cast<float>(x), static_cast<float>(y));
}

void SnakeSegment::setPosition(const Point& point) {
    setPosition(point.x, point.y);
}

Point SnakeSegment::getPosition() const {
    return position;
}

void SnakeSegment::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}
