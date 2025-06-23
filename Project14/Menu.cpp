#include "Menu.h"
#include "GameBoy.h"
#include <iostream>
#include "SnakeGame.h"
#include "Hangman-Game.h"
#include "Leaderboard.h"
#include "Wordle.h"
#include "Game.h"

Menu::Menu(sf::RenderWindow* window, InputSystem* inputSystem, GameBoy* gameBoy)
    : window(window), inputSystem(inputSystem), gameBoy(gameBoy) {
    if (!backgroundTexture.loadFromFile("blue_background.png")) {
        cout << "Error loading background image!" << endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    if (!font.loadFromFile("font/ARIAL.ttf")) {
        cout << "Error loading font!" << endl;
    }

    // Load button texture
    if (!buttonTexture.loadFromFile("button1.png")) {
        cout << "Error loading button image!" << endl;
    }
    buttonSprite.setTexture(buttonTexture);


    //// Welcome
    //welcomeTextLeft.setFont(font);
    //welcomeTextLeft.setString("Welcome!");
    //welcomeTextLeft.setCharacterSize(80);  
    //welcomeTextLeft.setFillColor(sf::Color::White);
    //welcomeTextLeft.setStyle(sf::Text::Bold);  // Bold style
    //welcomeTextLeft.setPosition(80, 300);  // Adjust position for left side

    //// GameBoy
    //welcomeTextRight.setFont(font);
    //welcomeTextRight.setString("GameBoy!");
    //welcomeTextRight.setCharacterSize(80);
    //welcomeTextRight.setFillColor(sf::Color::White);
    //welcomeTextRight.setStyle(sf::Text::Bold);
    //welcomeTextRight.setPosition(830, 300);  // Adjust position for right side

}

void Menu::draw() {
    window->clear(sf::Color::Black);  // Clear the screen
    window->draw(backgroundSprite);  // Draw the background



    drawButton("Snake Game", 490, 100, 300, 75);  // Button 1
    drawButton("Hangman Game", 490, 200, 300, 75);  // Button 2
    drawButton("Wordle Game", 490, 300, 300, 75);  // Button 3
    drawButton("LeaderBoard", 490, 400, 300, 75);  // Button 4
    drawButton("Exit", 490, 500, 300, 75);  // Button 5
}



void Menu::handleClick() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
       if (mousePos.x >= 490 && mousePos.x <= 790 && mousePos.y >= 100 && mousePos.y <= 175) 
       {
            cout << "Starting SnakeGame!" << endl;
            gameBoy->switchGame(new SnakeGame(window, inputSystem));  // Call switchGame
       }
       else if (mousePos.x >= 490 && mousePos.x <= 790 && mousePos.y >= 200 && mousePos.y <= 275)
       {
           cout << "Starting HangMan Game!" << endl;
           gameBoy->switchGame(new HangmanGame(window, inputSystem));  // Call switchGame
       }
       else if (mousePos.x >= 490 && mousePos.x <= 790 && mousePos.y >= 300 && mousePos.y <= 375)
       {
           cout << "Starting Wordle Game!" << endl;
           gameBoy->switchGame(new Wordle(window, inputSystem));  // Call switchGame
       }
       else if (mousePos.x >= 490 && mousePos.x <= 790 && mousePos.y >= 400 && mousePos.y <= 475)
       {
           cout << "LeaderBoard!" << endl;
           gameBoy->switchGame(new Leaderboard(window, inputSystem));
       }
        // Check button bounds for "Exit"
       else if (mousePos.x >= 490 && mousePos.x <= 790 && mousePos.y >= 500 && mousePos.y <= 575)
       {
            cout << "Exiting game!" << endl;
            window->close();  // Close the application
       }
    }
}




void Menu::drawButton(const string& label, float x, float y, float width, float height) {
    
    buttonSprite.setPosition(x, y);
    buttonSprite.setScale(width / 163.0f, height / 70.0f); // Scale proportionally

    window->draw(buttonSprite);

    // Draw the label text
    sf::Text text(label, font, 24);
    text.setFillColor(sf::Color::White);
    text.setPosition(x + (width - text.getGlobalBounds().width) / 2,
        y + (height - text.getGlobalBounds().height) / 2);
    window->draw(text);
}
