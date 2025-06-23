#pragma once
#include <SFML/Audio.hpp>
#include <string>
using namespace std;

class SoundManager {
public:
    SoundManager();
    void playMusic(int gameIndex, bool loop = true); 
    void stopMusic();

private:
    sf::Music currentMusic;
    const string musicFiles[3] = { "Snake_sound.ogg", "Hangman_sound.ogg", "Wordle_sound.ogg" };
};
