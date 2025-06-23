#pragma once
#include <SFML/Graphics.hpp>
#include "InputSystem.h"
#include "Game.h"
#include <SFML/Audio.hpp> 

class HangmanGame : public Game {
public:
    bool areStringsEqual(const char* str1, const char* str2);
    void copyString(char* dest, const char* src);
    void clearString(char* str, size_t length);
    HangmanGame(sf::RenderWindow* window, InputSystem* inputSystem);
    bool shouldQuitToMenu() const override;

    void startGame() ;     
    void render(sf::RenderWindow& window) override;
    void update() override;    

private:
    void handleInput();            
    bool quitToMenu; 
    sf::RenderWindow* window;      
    InputSystem* inputSystem;      

    sf::Font font;                 
    sf::Texture backgroundTexture; 
    sf::Sprite backgroundSprite;   
    sf::Clock gameClock;  
    int totalTime = 120; 
    int remainingTime = 120; 
    bool timeUp = false; 

    static const int maxWordLength = 10;
    char wordToGuess[maxWordLength];
    char guessedWord[maxWordLength]; 
    int wordLength;                 
    bool guessedLetters[26];       
    bool hasWon;
    int attemptsLeft;               
    int streak; 
    int score;        
    
    bool isGameOver;
};
