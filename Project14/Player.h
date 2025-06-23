#pragma once
#include <iostream>
#include <fstream>

class Player {
private:
    char name[50]; // Fixed-size char array for player name
    char achievements[10][50]; // Fixed-size array for up to 10 achievements
    int achievementCount = 0; // To track how many achievements the player has

public:
    Player(const char* playerName); // Constructor
    void saveProgress(const char* filename) const; // Save player's progress to a file
    void loadProgress(const char* filename); // Load player's progress from a file
    const char* getName() const; // Get the player's name
    void addAchievement(const char* achievement); // Add a new achievement
};
