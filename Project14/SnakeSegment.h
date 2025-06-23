#pragma once
#include <SFML/Graphics.hpp>
#include "Point.h"

class SnakeSegment {
public:
    SnakeSegment();               // Default constructor (for dynamic allocation)
    SnakeSegment(int x, int y);   // Parameterized constructor

    void setPosition(int x, int y);
    void setPosition(const Point& point);
    Point getPosition() const;
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite sprite;            // Visual representation of the segment
    static sf::Texture texture;   // Shared texture for all segments
    Point position;               // Logical position of the segment
};
