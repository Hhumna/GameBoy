#pragma once
#include <SFML/Graphics.hpp>

class Grid {
public:
    Grid(int columns, int rows);  // Constructor with grid dimensions
    void draw(sf::RenderWindow& window);

    int getWidth() const { return columns; }  // Getter for width
    int getHeight() const { return rows; }   // Getter for height

private:
    int columns;  // Number of vertical cells
    int rows;     // Number of horizontal cells
    static const int cellSize = 40;  // Cell size (in pixels)
};
