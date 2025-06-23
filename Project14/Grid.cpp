#include "Grid.h"

Grid::Grid(int columns, int rows) : columns(columns), rows(rows) {}

void Grid::draw(sf::RenderWindow& window) {
    sf::VertexArray lines(sf::Lines);

    // Draw horizontal lines
    for (int i = 0; i <= rows; ++i) {
        lines.append(sf::Vertex(sf::Vector2f(0, i * cellSize), sf::Color::White));
        lines.append(sf::Vertex(sf::Vector2f(columns * cellSize, i * cellSize), sf::Color::White));
    }

    // Draw vertical lines
    for (int i = 0; i <= columns; ++i) {
        lines.append(sf::Vertex(sf::Vector2f(i * cellSize, 0), sf::Color::White));
        lines.append(sf::Vertex(sf::Vector2f(i * cellSize, rows * cellSize), sf::Color::White));
    }

    window.draw(lines);
}
