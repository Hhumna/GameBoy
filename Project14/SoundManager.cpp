#include "SoundManager.h"
#include <iostream>

SoundManager::SoundManager() {}

void SoundManager::playMusic(int gameIndex, bool loop) {
    if (gameIndex < 0 || gameIndex >= 3) {
        cerr << "Invalid game index!" << endl;
        return;
    }

    // Stop any currently playing music
    stopMusic();

    // Try to load and play the selected music
    if (!currentMusic.openFromFile(musicFiles[gameIndex])) {
        cerr << "Error loading music file: " << musicFiles[gameIndex] << endl;
        return;
    }

    currentMusic.setLoop(loop);  // Loop the music if specified
    currentMusic.play();         // Start playing the music
}

void SoundManager::stopMusic() {
    currentMusic.stop();  // Stop the music
}
