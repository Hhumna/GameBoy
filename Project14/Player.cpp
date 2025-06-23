#include "Player.h"
#include <fstream>
#include <iostream>

// Function to manually copy one char array to another
void copyCharArray2(const char* source, char* destination, int size) {
    int i = 0;
    while (source[i] != '\0' && i < size - 1) {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0'; // Null-terminate the destination
}

Player::Player(const char* playerName) {
    copyCharArray2(playerName, name, 50);
}

void Player::saveProgress(const char* filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file for saving progress.\n";
        return;
    }
    file << name << '\n';
    for (int i = 0; i < achievementCount; i++) {
        file << achievements[i] << '\n';
    }
}

void Player::loadProgress(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file to load progress.\n";
        return;
    }

    achievementCount = 0;
    file.getline(name, 50);
    while (file.getline(achievements[achievementCount], 50)) {
        achievementCount++;
    }
}

const char* Player::getName() const {
    return name;
}

void Player::addAchievement(const char* achievement) {
    if (achievementCount < 10) {
        copyCharArray2(achievement, achievements[achievementCount], 50);
        achievementCount++;
    }
}
