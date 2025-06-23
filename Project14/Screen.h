#pragma once
#include <SFML/Graphics.hpp>

class Screen {
public:
    Screen(int width, int height, const std::string& title);
    void clear(sf::Color color = sf::Color::Black);  // Clear the screen with the given color
    void render(sf::Drawable& drawable);             // Render any drawable object
    void display();                                  // Display the rendered frame
    bool isOpen();                                   // Check if the window is still open
    void close();                                    // Close the window

    // Add this accessor method
    sf::RenderWindow& getWindow();  // Get the underlying sf::RenderWindow

private:
    sf::RenderWindow window;
};
