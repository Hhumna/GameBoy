#include "Hangman-Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

// Compare two character arrays
bool HangmanGame :: areStringsEqual(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return false;
        }
        ++str1;
        ++str2;
    }
    return *str1 == '\0' && *str2 == '\0';
}


void HangmanGame :: copyString(char* dest, const char* src) {
    while (*src) {
        *dest = *src;
        ++dest;
        ++src;
    }
    *dest = '\0';
}


void HangmanGame :: clearString(char* str, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        str[i] = '\0';
    }
}

// Main class implementation
HangmanGame::HangmanGame(sf::RenderWindow* sharedWindow, InputSystem* input)
    : window(sharedWindow), inputSystem(input), isGameOver(false), attemptsLeft(6), score(0), streak(0), hasWon(false), remainingTime(120), timeUp(false), quitToMenu(false){
    if (!font.loadFromFile("font/ARIAL.ttf")) {
        cerr << "Error loading font!" << endl;
    }

    if (!backgroundTexture.loadFromFile("Hangman-Background.jpg")) {
        cerr << "Error loading background texture!" << endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    float scaleX = static_cast<float>(1280) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(720) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);

    startGame();
}

bool HangmanGame::shouldQuitToMenu() const {
    return quitToMenu;
}


void HangmanGame::startGame() {
    const char wordBank[][10] = { "program", "hangman", "game", "sfml", "coding" };
    int wordCount = 5;
    srand(static_cast<unsigned>(time(nullptr)));
    int randomIndex = rand() % wordCount;

    clearString(wordToGuess, 10);
    clearString(guessedWord, 10);

    // Copy word from wordBank to wordToGuess
    copyString(wordToGuess, wordBank[randomIndex]);

    // Initialize guessedWord with underscores
    for (int i = 0; wordToGuess[i] != '\0'; ++i) {
        guessedWord[i] = '_';
    }

    wordLength = 0;
    while (wordToGuess[wordLength] != '\0') {
        ++wordLength;
    }

    attemptsLeft = 6;
    for (int j = 0; j < 26; ++j) {
        guessedLetters[j] = false;
    }
    isGameOver = false;
    hasWon = false;
    timeUp = false;

}
void HangmanGame::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
        // Restart the game immediately (within time)
        if (remainingTime > 0) {
            if (hasWon) {
                streak++;
            }
            startGame();
        }
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        
        
        streak = 0;
        score = 0;
          
        remainingTime = totalTime;
        gameClock.restart();
        startGame();


        ////////////////////////////////-- File Handling--////////////////////////////////

        // Save score to file if its new highest Score

        ifstream fileIn("LeaderboardData.txt"); // Open the file for reading
        if (!fileIn.is_open()) {
            cerr << "Error: Could not open file for reading.\n";
            return;
        }

        // Check if the file is empty
        fileIn.seekg(0, ios::end); // Move to the end of the file
        bool isEmpty = fileIn.tellg() == 0; // Check if the current position is 0 (empty file)
        fileIn.seekg(0, ios::beg); // Reset position to the start of the file

        if (isEmpty) {
            fileIn.close();

            // Write default format to the file
            ofstream fileOut("LeaderboardData.txt");
            if (!fileOut.is_open()) {
                cerr << "Error: Could not open file for writing.\n";
                return;
            }
            fileOut << "Player: Humna\n";
            fileOut << "Snake Game: 0 points\n";
            fileOut << "Hangman Game: " << score << " points\n";
            fileOut << "Wordle Game: 0 points\n";
            fileOut.close();
            cout << "File was empty. Initialized with default format.\n";
            return;
        }

        // If the file is not empty, process it normally
        ostringstream tempBuffer;
        string line;
        bool hangmanScoreUpdated = false;

        // Read the file line by line
        while (getline(fileIn, line)) {
            if (line.find("Snake Game") != string::npos) {
                // Find the position of the colon (:) and the space after it
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    // Extract the substring after the colon and space
                    size_t start = line.find_first_not_of(' ', pos + 1);  // Skip space after ':'
                    size_t end = line.find(' ', start); // Find the next space after the number

                    // If the end space isn't found, it means the score is the last number in the line
                    if (end == string::npos) {
                        end = line.length();  // Take till the end of the line
                    }

                    // Extract the score
                    string currentScore = line.substr(start, end - start);

                    // Compare scores and update if new score is higher
                    if (stoi(currentScore) < score) {
                        line = "Snake Game: " + to_string(score) + " points";
                        hangmanScoreUpdated = true;
                    }
                }
            }

            // Store the line in a temporary buffer
            tempBuffer << line << "\n";
        }

        fileIn.close(); // Close the input file

        if (hangmanScoreUpdated) {
            // Write updated content back to the file
            ofstream fileOut("LeaderboardData.txt");
            if (!fileOut.is_open()) {
                cout << "Error: Could not open file for writing.\n";
                return;
            }
            fileOut << tempBuffer.str();
            fileOut.close();
            cout << "High score updated successfully!\n";
        }
        else {
            cout << "No update needed: current high score is greater or equal.\n";
        }

        cout << "Game exited! Score and streak saved to the leaderboard.\n";




        
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        // Quit to menu
        quitToMenu = true;

        // Handle leaderboard updates
        ifstream fileIn("LeaderboardData.txt");
        if (!fileIn.is_open()) {
            cerr << "Error: Could not open file for reading.\n";
            return;
        }

        // Check if the file is empty
        fileIn.seekg(0, ios::end);
        bool isEmpty = fileIn.tellg() == 0;
        fileIn.seekg(0, ios::beg);

        if (isEmpty) {
            fileIn.close();
            ofstream fileOut("LeaderboardData.txt");
            if (!fileOut.is_open()) {
                cerr << "Error: Could not open file for writing.\n";
                return;
            }
            fileOut << "Player: Humna\n";
            fileOut << "Snake Game: 0 points\n";
            fileOut << "Hangman Game: " << score << " points\n";
            fileOut << "Wordle Game: 0 points\n";
            fileOut.close();
            cout << "File was empty. Initialized with default format.\n";
            return;
        }

        // Process the file for leaderboard updates
        ostringstream tempBuffer;
        string line;
        bool hangmanScoreUpdated = false;

        while (getline(fileIn, line)) {
            if (line.find("Hangman Game") != string::npos) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    size_t start = line.find_first_not_of(' ', pos + 1);
                    size_t end = line.find(' ', start);
                    if (end == string::npos) end = line.length();

                    string currentScore = line.substr(start, end - start);
                    if (stoi(currentScore) < score) {
                        line = "Hangman Game: " + to_string(score) + " points";
                        hangmanScoreUpdated = true;
                    }
                }
            }
            tempBuffer << line << "\n";
        }

        fileIn.close();

        if (hangmanScoreUpdated) {
            ofstream fileOut("LeaderboardData.txt");
            if (!fileOut.is_open()) {
                cout << "Error: Could not open file for writing.\n";
                return;
            }
            fileOut << tempBuffer.str();
            fileOut.close();
            cout << "High score updated successfully!\n";
        }
        else {
            cout << "No update needed: current high score is greater or equal.\n";
        }

        cout << "Game exited! Score and streak saved to the leaderboard.\n";
        return;
    }

    if (isGameOver) return;

    // Handle alphabet key presses
    for (char c = 'A'; c <= 'Z'; ++c) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(c - 'A' + sf::Keyboard::A))) {
            char guessedChar = static_cast<char>(tolower(c));
            int letterIndex = guessedChar - 'a';

            if (!guessedLetters[letterIndex]) {
                guessedLetters[letterIndex] = true;
                bool correct = false;

                for (int i = 0; i < wordLength; ++i) {
                    if (wordToGuess[i] == guessedChar) {
                        guessedWord[i] = guessedChar;
                        correct = true;
                        score += 10; // Increment score for each correct character
                    }
                }

                if (!correct) {
                    --attemptsLeft;
                }

                if (areStringsEqual(guessedWord, wordToGuess)) {
                    streak++;// Increment streak if the word is guessed correctly
                }
                
                if (attemptsLeft == 0) {
                    streak = 0; // Reset streak if game ends
                    isGameOver = true;  // Game over after 6 attempts
                }

                isGameOver = attemptsLeft == 0 || areStringsEqual(guessedWord, wordToGuess);
                break;
            }
        }
    }
}
void HangmanGame::update() {
    handleInput();

    if (isGameOver) return;

    int elapsedTime = gameClock.getElapsedTime().asSeconds();
    remainingTime = totalTime - elapsedTime;

    if (remainingTime <= 0) {
        remainingTime = 0;
        isGameOver = true;
        timeUp = true; // Mark the game as ended due to timeout
        streak = 0; // Reset the streak when time is up
    }
}


// Rest of the code (rendering logic, etc.) remains the same.

void HangmanGame::render(sf::RenderWindow& window) {
    window.clear();
    window.draw(backgroundSprite);

    // Calculate remaining time
    int elapsedTime = gameClock.getElapsedTime().asSeconds();
    int remainingTime = totalTime - elapsedTime;

    // Display remaining time
    char timerTextBuffer[50];
    sprintf_s(timerTextBuffer, "Time Left: %02d:%02d", remainingTime / 60, remainingTime % 60);
    sf::Text timerText(timerTextBuffer, font, 30);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10, 40);
    window.draw(timerText);


    // Hangman structure on the left side
    sf::RectangleShape hangmanBase(sf::Vector2f(200, 10));
    hangmanBase.setPosition(100, 600);
    hangmanBase.setFillColor(sf::Color::White);
    window.draw(hangmanBase);

    sf::RectangleShape gallowsVertical(sf::Vector2f(10, 285));
    gallowsVertical.setPosition(150, 310);
    gallowsVertical.setFillColor(sf::Color::White);
    window.draw(gallowsVertical);

    sf::RectangleShape gallowsHorizontal(sf::Vector2f(100, 10));
    gallowsHorizontal.setPosition(150, 300);
    gallowsHorizontal.setFillColor(sf::Color::White);
    window.draw(gallowsHorizontal);

    sf::RectangleShape rope(sf::Vector2f(10, 50));
    rope.setPosition(240, 310);
    rope.setFillColor(sf::Color::White);
    window.draw(rope);

    // Draw hangman parts based on attempts left
    if (attemptsLeft <= 5) {
        sf::CircleShape head(30);
        head.setFillColor(sf::Color::Transparent);
        head.setOutlineThickness(5);
        head.setOutlineColor(sf::Color::White);
        head.setPosition(210, 360);
        window.draw(head);
    }
    if (attemptsLeft <= 4) {
        sf::RectangleShape body(sf::Vector2f(10, 50));
        body.setPosition(240, 420);
        body.setFillColor(sf::Color::White);
        window.draw(body);
    }
    if (attemptsLeft <= 3) {
        // Left arm
        sf::RectangleShape leftArm(sf::Vector2f(45, 10));
        leftArm.setPosition(240, 440);
        leftArm.setFillColor(sf::Color::White);
        leftArm.rotate(135); // Left arm angled upwards
        window.draw(leftArm);
    }
    if (attemptsLeft <= 2) {
        // Right arm
        sf::RectangleShape rightArm(sf::Vector2f(43, 10));
        rightArm.setPosition(260, 430);
        rightArm.setFillColor(sf::Color::White);
        rightArm.rotate(45); // Right arm angled upwards
        window.draw(rightArm);
    }
    if (attemptsLeft <= 1) {
        // Left leg
        sf::RectangleShape leftLeg(sf::Vector2f(45, 10));
        leftLeg.setPosition(240, 470);
        leftLeg.setFillColor(sf::Color::White);
        leftLeg.rotate(135); // Left leg angled downward
        window.draw(leftLeg);
    }
    if (attemptsLeft == 0) {
        // Right leg
        sf::RectangleShape rightLeg(sf::Vector2f(45, 10));
        rightLeg.setPosition(255, 465);
        rightLeg.setFillColor(sf::Color::White);
        rightLeg.rotate(45); // Right leg angled downward
        window.draw(rightLeg);
    }

    // Display word and attempts on the right side
    float blankStartX = 730;
    float blankStartY = 350;
    float blankSpacing = 50;

    for (int i = 0; i < wordLength; ++i) {
        sf::RectangleShape blank(sf::Vector2f(40, 5));
        blank.setFillColor(sf::Color::White);
        blank.setPosition(blankStartX + i * blankSpacing, blankStartY);
        window.draw(blank);

        if (guessedWord[i] != '_') {
            sf::Text letter(string(1, guessedWord[i]), font, 40);
            letter.setFillColor(sf::Color::White);
            letter.setPosition(blankStartX + i * blankSpacing + 10, blankStartY - 50);
            window.draw(letter);
        }
    }

    sf::Text attemptsText("Attempts Left: " + to_string(attemptsLeft), font, 30);
    attemptsText.setFillColor(sf::Color::Red);
    attemptsText.setPosition(10, 7);
    window.draw(attemptsText);

    sf::Text scoreText("Score: " + to_string(score), font, 30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(1140, 7);
    window.draw(scoreText);

    sf::Text streakText("Streak: " + to_string(streak), font, 30);
    streakText.setFillColor(sf::Color::White);
    streakText.setPosition(1140, 40);
    window.draw(streakText);

    // Draw the visual keyboard
   // Draw the visual keyboard
    float keyWidth = 40;
    float keyHeight = 40;
    float startX = 730;
    float startY = 500;
    int keyGap = 5;

    for (char c = 'A'; c <= 'Z'; ++c) {
        int index = c - 'A';
        int row = index / 10;
        int col = index % 10;

        sf::RectangleShape key(sf::Vector2f(keyWidth, keyHeight));
        key.setPosition(startX + col * (keyWidth + keyGap), startY + row * (keyHeight + keyGap));

       
        if (!guessedLetters[index]) {
            key.setFillColor(sf::Color::White); // Not guessed yet
        }
        else {
            // Check if the guessed letter is in the word
            bool isCorrect = false;
            for (int i = 0; i < wordLength; ++i) {
                if (wordToGuess[i] == tolower(c)) {
                    isCorrect = true;
                    break;
                }
            }
            key.setFillColor(isCorrect ? sf::Color::Green : sf::Color::Red); // Correct: Green, Incorrect: Red
        }

        window.draw(key);

        // Draw the letter on the key
        sf::Text keyText(string(1, c), font, 20);
        keyText.setFillColor(sf::Color::Black);
        keyText.setPosition(startX + col * (keyWidth + keyGap) + 10, startY + row * (keyHeight + keyGap) + 5);
        window.draw(keyText);
    }

    if (isGameOver) {
        if (timeUp) {
            sf::Text timeUpText("Time's Up! Game Over!", font, 50);
            timeUpText.setFillColor(sf::Color::Yellow);
            timeUpText.setPosition(400, 200);
            window.draw(timeUpText);

            sf::Text restartOrQuitText("Press Space to Restart or Esc to Quit", font, 30);
            restartOrQuitText.setFillColor(sf::Color::White);
            restartOrQuitText.setPosition(400, 300);
            window.draw(restartOrQuitText);
        }
        else {
            sf::Text gameOverText(hasWon ? "YOU WIN!" : "GAME OVER!", font, 50);
            gameOverText.setFillColor(hasWon ? sf::Color::Green : sf::Color::Red);
            gameOverText.setPosition(450, 200);
            window.draw(gameOverText);

            sf::Text restartText("Press Tab to Restart or Esc to Quit", font, 40);
            restartText.setFillColor(sf::Color::White);
            restartText.setPosition(330, 260);
            window.draw(restartText);
        }
    }


  //  window.display();
}

