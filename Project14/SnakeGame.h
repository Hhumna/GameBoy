#pragma once
#include <SFML/Graphics.hpp>
#include "Point.h"
#include "SnakeSegment.h"
#include "Grid.h"
#include "InputSystem.h"
#include "Game.h" 
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

enum class Direction { Up, Down, Left, Right };

class SnakeGame : public Game {
public:
    SnakeGame(sf::RenderWindow* window, InputSystem* inputSystem);

    void startGame() override;
    void render(sf::RenderWindow& window) override;
    void update() override;
    bool shouldQuitToMenu() const override;

private:
    void handleInput();               
    void checkSelfCollision();      
    void checkBorderCollision();    
    bool checkFoodCollision();            
    void restartGame();              
    void loadFont();
    void loadTextures();
    void generateFood();
    void generatePowerUp();
    void checkPowerUpCollision();
    

    sf::RenderWindow* window;
    InputSystem* inputSystem;
    static const int MAX_SNAKE_LENGTH = 500;
    SnakeSegment snake[MAX_SNAKE_LENGTH];
    int currentLength;
    Direction direction;
    Point food;
    Grid grid;
    bool isGameOver;
    bool isPaused;
    bool pausePressed;
    bool quitToMenu; 
    int lastScoreForSpeedChange; 
    //Direction lastDirection;  // Tracks the last direction used
    int level;

    sf::Font font;
    sf::Text scoreText;
    sf::Text gameOverText;

    sf::Texture foodTexture;          // Texture for the food image
    sf::Sprite foodSprite;            // Sprite for displaying the food
    sf::Texture backgroundTexture;    // Texture for the background image
    sf::Sprite backgroundSprite;      // Sprite for displaying the background

    sf::Text speedText;

    int score;
    const int cellSize = 40;

    sf::Clock clock1;                 // Clock for tracking time
    float movementTimeAccumulator = 0.0f;
    float timeStep = 0.1f;

    // In your class
    sf::Clock gameClock;
    sf::Time elapsedTime;



    sf::Texture powerUpTexture;       // Texture for the power-up image
    sf::Sprite powerUpSprite;         // Sprite to display the power-up
    bool isPowerUpActive;             // Whether the power-up is currently active
    Point powerUpPosition;            // Logical position of the power-up
    sf::Clock powerUpSpawnTimer;      // Timer to control power-up spawning


};
