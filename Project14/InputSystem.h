#pragma once
#include <SFML/Window.hpp>

class InputSystem {
public:
    InputSystem();
    void handleInput();  
    bool isKeyPressed(sf::Keyboard::Key key); 
    sf::Keyboard::Key getDirection() const; 

private:
    sf::Keyboard::Key direction;
};
