#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "InputSystem.h"
#include "Game.h"


class Leaderboard :public Game {
    string playerName;
    int snakeScore = 0;
    int hangmanScore = 0;
    int wordleScore = 0;
    bool quitToMenu;

    sf::RenderWindow* window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    Leaderboard(sf::RenderWindow* sharedWindow, InputSystem* input);

    void startGame() override; // Initialize/restart the game
    void update() override; // Update game logic
    void render(sf::RenderWindow& window) override; // Render the game};
    bool shouldQuitToMenu() const override;
};

