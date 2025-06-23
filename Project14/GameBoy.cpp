#include "GameBoy.h"
#include "SnakeGame.h" 
#include "Hangman-Game.h"
#include "Wordle.h"

GameBoy::GameBoy()
    : screen(1280, 720, "GameBoy"), currentGame(nullptr), isMenuActive(true) {
    menu = new Menu(&screen.getWindow(), &inputSystem, this);  // Passing the window from screen to menu
}

void GameBoy::start() {
    while (screen.isOpen()) {
        sf::Event event;
        while (screen.getWindow().pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                screen.close();
            }
        }

        if (isMenuActive) {
            soundManager.stopMusic();  
            menu->draw();  
            menu->handleClick();
        }
        else if (currentGame) {             // check for quit game from current game here
            currentGame->update();
            if (currentGame->shouldQuitToMenu()) {
                soundManager.stopMusic(); 
                isMenuActive = true;
                delete currentGame;
                currentGame = nullptr;
                continue;
            }
            currentGame->render(screen.getWindow());
        }

        screen.display();
    }
}

void GameBoy::switchGame(Game* newGame) {
    if (currentGame) {
        delete currentGame;
        cout << "Previous game deleted." << endl;
    }
    else {
        cout << "No previous game to delete. Starting first game..." << endl;
    }

    currentGame = newGame;  
    currentGame->startGame();  

    isMenuActive = false;  // Deactivate the menu
    cout << "Switched to game. Menu is now inactive." << endl;

    // Play music for the current game
    if (currentGame) {
        if (dynamic_cast<SnakeGame*>(currentGame)) {
            soundManager.playMusic(0); // Play Snake game music (index 0)
        }
        else if (dynamic_cast<HangmanGame*>(currentGame)) {
            soundManager.playMusic(1); // Play Hangman game music (index 1)
        }
        else if (dynamic_cast<Wordle*>(currentGame)) {
            soundManager.playMusic(2); // Play Wordle game music (index 2)
        }
    }
}
