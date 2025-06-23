#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Game.h"
#include "InputSystem.h"
#include "Screen.h"  
#include <SFML/Audio.hpp>  
#include "SoundManager.h"

class GameBoy {
public:
    GameBoy();                    
    void start();                 
    void switchGame(Game* newGame);  
private:
    Screen screen;                
    InputSystem inputSystem;      
    Menu* menu;                   
    Game* currentGame;            
    bool isMenuActive;            
    SoundManager soundManager;
};
