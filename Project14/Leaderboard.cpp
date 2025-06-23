#include "Leaderboard.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include<fstream>
#include<sstream>


Leaderboard::Leaderboard(sf::RenderWindow* sharedWindow, InputSystem* input)
    : window(sharedWindow), quitToMenu(false) {
    // Load the font
    if (!font.loadFromFile("font/ARIAL.ttf")) {
        cerr << "Error loading font!\n";
    }

    // Load the background texture
    if (!backgroundTexture.loadFromFile("background1.png")) {
        cerr << "Error loading background texture!\n";
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Scale the background to fit the window size
    float scaleX = static_cast<float>(1280) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(720) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);
    startGame();
}



// Initialize or reset the game
void Leaderboard::startGame() {
    update();

}

bool Leaderboard::shouldQuitToMenu() const {
    return quitToMenu;
}


void Leaderboard::render(sf::RenderWindow& window) {
    window.clear();
    window.draw(backgroundSprite); // Draw the background

    ifstream file("LeaderboardData.txt"); // Open the file for reading
    if (!file.is_open()) {
        cerr << "Error: Could not open LeaderboardData.txt.\n";
        return;
    }

    string line;
    float yOffset = 160.0f; // Start displaying text at this Y coordinate


    // Read the file line by line and render each line to both the SFML window and the console
    while (getline(file, line)) {
 
        // Render each line to the SFML window
        sf::Text text(line, font, 25);  // Create sf::Text object for each line
        text.setFillColor(sf::Color::Black);  // Set text color (white)
        text.setPosition(150, yOffset);  // Set position for each line
        window.draw(text);  // Draw the text to the window

        yOffset += 30.0f;  // Move the next line down
    }

    file.close();  // Close the file after reading


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

        quitToMenu = true;  // Signal to return to the menu
    }

}


void Leaderboard::update() {

}