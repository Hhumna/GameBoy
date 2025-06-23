#include "Screen.h"

Screen::Screen(int width, int height, const std::string& title) {
    window.create(sf::VideoMode(width, height), title);
    window.setFramerateLimit(60);  // Cap the frame rate to 60 FPS
}

void Screen::clear(sf::Color color) {
    window.clear(color);
}

void Screen::render(sf::Drawable& drawable) {
    window.draw(drawable);  // Draw anything drawable (e.g., Snake, menu button)
}

void Screen::display() {
    window.display();  // Display the rendered frame
}

bool Screen::isOpen() {
    return window.isOpen();
}

void Screen::close() {
    window.close();  // Close the window
}

// Add this to access the window object
sf::RenderWindow& Screen::getWindow() {
    return window;
}
