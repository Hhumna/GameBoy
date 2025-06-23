#include "InputSystem.h"

InputSystem::InputSystem() : direction(sf::Keyboard::Right) {}

void InputSystem::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != sf::Keyboard::Down) {
        direction = sf::Keyboard::Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != sf::Keyboard::Up) {
        direction = sf::Keyboard::Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != sf::Keyboard::Right) {
        direction = sf::Keyboard::Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != sf::Keyboard::Left) {
        direction = sf::Keyboard::Right;
    }
}

bool InputSystem::isKeyPressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);  // Check if a specific key is pressed
}

sf::Keyboard::Key InputSystem::getDirection() const {
    return direction;
}
