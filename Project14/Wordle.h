#pragma once
#include "Game.h"
#include <SFML/Graphics.hpp>
#include "InputSystem.h"
#include <SFML/Audio.hpp> 



class Wordle : public Game{
public:
    // Constructor
    Wordle(sf::RenderWindow* sharedWindow, InputSystem* input);

    // Game lifecycle methods
    void startGame(); // Initialize/restart the game
    void handleInput(); // Handle user inputs
    void update(); // Update game logic
    void render(sf::RenderWindow& window); // Render the game
    bool shouldQuitToMenu() const override;
    int score;
    int streak;

private:
    // Utility functions
    bool areStringsEqual(const char* str1, const char* str2); // Compare two strings
    void copyString(char* dest, const char* src); // Copy a string
    void clearString(char* str, size_t length); // Clear a string

    void processAttempt(); // Process the current guess attempt

    // Member variables
    sf::RenderWindow* window; // Shared render window
    InputSystem* inputSystem; // Input handling system

    sf::Font font; // Font for text rendering
    sf::Texture backgroundTexture; // Background image
    sf::Sprite backgroundSprite; // Background sprite

    int attemptsLeft; // Remaining attempts
    int currentAttempt; // Current row in the grid
    int currentColumn; // Current column in the grid

    char wordToGuess[6]; // Target word
    char guessedWord[6]; // Current guessed word
    char guessedLetters[6][5]; // Grid of guessed letters
    int currentRow = 0;          // Current row being guessed
    int currentCol = 0;          // Current column in the row
   // std::string wordToGuess = "CRANE"; // The correct word to guess
    bool isGameOver = false;     // Whether the game is over
    bool hasWon;
    sf::Color keyColors[26];     // Colors for each letter on the keyboard

    sf::Color gridColors[6][5]; // Colors of the grid cells

    bool quitToMenu;
};
