#include "SnakeGame.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructor
SnakeGame::SnakeGame(sf::RenderWindow* sharedWindow, InputSystem* input)
    : window(sharedWindow), inputSystem(input), cellSize(40), isGameOver(false), isPaused(false),
    quitToMenu(false), grid(1280 / 40, 720 / 40), direction(Direction::Right), currentLength(3),
    score(0), movementTimeAccumulator(0.0f), timeStep(0.1f), isPowerUpActive(false) {

    loadFont();
    loadTextures();
    startGame();
}


// Load font for text rendering
void SnakeGame::loadFont() {
    if (!font.loadFromFile("font/ARIAL.ttf")) {
        cerr << "Error loading font!" << endl;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 20);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Game Over! Press SPACE to Restart");
    gameOverText.setPosition(310, 300);

    speedText.setFont(font);
    speedText.setCharacterSize(30);
    speedText.setFillColor(sf::Color::White);
    speedText.setPosition(580, 20);  

}

// Load textures for background and food
void SnakeGame::loadTextures() {
    if (!foodTexture.loadFromFile("cheery2.png")) {
        cerr << "Error loading food texture!" << endl;
    }
    foodSprite.setTexture(foodTexture);
    foodSprite.setScale(static_cast<float>(cellSize) / foodTexture.getSize().x,
        static_cast<float>(cellSize) / foodTexture.getSize().y);

    if (!backgroundTexture.loadFromFile("snake_background.png")) {
        cerr << "Error loading background texture!" << endl;
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(1280) / backgroundTexture.getSize().x,
        static_cast<float>(720) / backgroundTexture.getSize().y);

    // Load power-up texture
    if (!powerUpTexture.loadFromFile("powerup.png")) {  
        cerr << "Error loading power-up texture!" << endl;
    }
    powerUpSprite.setTexture(powerUpTexture);
    powerUpSprite.setScale(static_cast<float>(cellSize) / powerUpTexture.getSize().x,
        static_cast<float>(cellSize) / powerUpTexture.getSize().y);


}

bool SnakeGame::shouldQuitToMenu() const {
    return quitToMenu;
}


// Start a new game
void SnakeGame::startGame() {

    gameClock.restart();
    isGameOver = false;
    isPaused = false;
    score = 0;
    currentLength = 3;
    direction = Direction::Right;
    movementTimeAccumulator = 0.0f;
    timeStep = 0.1f;
    lastScoreForSpeedChange = 0;  // Reset when game starts
    for (int i = 0; i < currentLength; ++i) {
        snake[i] = SnakeSegment((grid.getWidth() / 2 - i) * cellSize, (grid.getHeight() / 2) * cellSize);
    }
    generateFood();
    clock1.restart();
}


// Restart the game
void SnakeGame::restartGame() {
    cout << "Restarting game..." << endl;
    for (int i = currentLength; i < MAX_SNAKE_LENGTH; ++i) {
        snake[i].setPosition(-cellSize, -cellSize);  // Move unused segments off-screen
    }
    startGame();
}

void SnakeGame::handleInput() {
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && !pausePressed) {
        isPaused = !isPaused;  // Toggle the pause state
        pausePressed = true;
        if (isPaused) {
            cout << "Game Paused" << endl;
        }
        else {
            cout << "Game Resumed" << endl;
            clock1.restart();  // Reset clock to avoid time jump after resuming
        }
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
        pausePressed = false; 
    }

    if (isPaused || isGameOver) return;  // Skip input handling if paused or game over

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != Direction::Down) {
        direction = Direction::Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != Direction::Up) {
        direction = Direction::Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != Direction::Right) {
        direction = Direction::Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != Direction::Left) {
        direction = Direction::Right;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

        quitToMenu = true;  // Signal to return to the menu

        

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

    }
}

void SnakeGame::update() {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        //updateLeaderBoard("Snake Game", score);
        restartGame();

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

    }

    handleInput();

    if (isPaused || isGameOver) return;

    generatePowerUp();  
    checkPowerUpCollision(); 

    // Get elapsed time since the game started
    elapsedTime = gameClock.getElapsedTime();


    // Increase speed 
    if (score >= lastScoreForSpeedChange + 150) {
        timeStep = max(0.05f, timeStep - 0.01f);  
        lastScoreForSpeedChange = score; 
        cout << "Speed increased! New timeStep: " << timeStep << endl;
    }

    float deltaTime = clock1.restart().asSeconds();
    movementTimeAccumulator += deltaTime;

    if (movementTimeAccumulator >= timeStep) {
        movementTimeAccumulator -= timeStep;

        // Move the snake body
        for (int i = currentLength - 1; i > 0; --i) {
            snake[i].setPosition(snake[i - 1].getPosition());
        }

        // Move the head
        Point newHead = snake[0].getPosition();
        if (direction == Direction::Up) newHead.y -= cellSize;
        else if (direction == Direction::Down) newHead.y += cellSize;
        else if (direction == Direction::Left) newHead.x -= cellSize;
        else if (direction == Direction::Right) newHead.x += cellSize;

        snake[0].setPosition(newHead);
        checkBorderCollision();

        

        if (!checkFoodCollision()) {
            checkSelfCollision();
        }

    }
}





void SnakeGame::generateFood() {
    bool foodIsOnSnake = true;  // Flag to check if food overlaps with snake

    while (foodIsOnSnake) {
        // Generate random position for the food
        food.setPosition((rand() % grid.getWidth()) * cellSize,
            (rand() % ((720 - 120) / cellSize)) * cellSize + 120);
        foodSprite.setPosition(static_cast<float>(food.x), static_cast<float>(food.y));

        // Check if food overlaps with any part of the snake
        foodIsOnSnake = false;  
        for (int i = 0; i < currentLength; ++i) {
            if (snake[i].getPosition() == food) {
                foodIsOnSnake = true;  
                break;  
            }
        }
    }
}


// Check food collision
bool SnakeGame::checkFoodCollision() {
    if (snake[0].getPosition() == food) {
        if (currentLength < MAX_SNAKE_LENGTH) {
            if (currentLength < MAX_SNAKE_LENGTH) {
                Point lastSegmentPosition = snake[currentLength - 1].getPosition();
                snake[currentLength++] = SnakeSegment(lastSegmentPosition.x, lastSegmentPosition.y);
            }

        }
        score += 10;
        generateFood();
        return true;
    }
    return false;
}

// Generate new power-up within bounds and ensure it doesn't overlap with the snake or food
void SnakeGame::generatePowerUp() {
    if (!isPowerUpActive && powerUpSpawnTimer.getElapsedTime().asSeconds() > 30) {  // Spawn every 30 seconds
        bool isPowerUpOnSnakeOrFood = true;  // Flag to check if power-up overlaps with snake or food

        while (isPowerUpOnSnakeOrFood) {
            // Generate random position for the power-up
            powerUpPosition.setPosition(
                (rand() % grid.getWidth()) * cellSize,
                (rand() % ((720 - 120) / cellSize)) * cellSize + 120
            );
            powerUpSprite.setPosition(static_cast<float>(powerUpPosition.x), static_cast<float>(powerUpPosition.y));

            // Check if power-up overlaps with any part of the snake
            isPowerUpOnSnakeOrFood = false;  // Assume power-up is not on the snake or food
            for (int i = 0; i < currentLength; ++i) {
                // Check if the power-up is on a snake segment
                if (snake[i].getPosition().x == powerUpPosition.x && snake[i].getPosition().y == powerUpPosition.y) {
                    isPowerUpOnSnakeOrFood = true;  // Power-up is on the snake, so regenerate
                    break;  // Exit early to regenerate the power-up
                }
            }

            // Check if power-up overlaps with food
            if (food.x == powerUpPosition.x && food.y == powerUpPosition.y) {
                isPowerUpOnSnakeOrFood = true;  // Power-up is on the food, so regenerate
            }
        }

        // Mark the power-up as active and restart the spawn timer
        isPowerUpActive = true;
        powerUpSpawnTimer.restart();
    }
}



void SnakeGame::checkPowerUpCollision() {
    if (isPowerUpActive && snake[0].getPosition() == powerUpPosition) {
        cout << "Power-Up Collected!" << endl;

        // Increase the score
        score += 50;
        int newLength = (currentLength + currentLength) / 3; // Calculate new length
        cout << currentLength;
        cout << newLength;
        // Ensure the snake length is at least 3
        if (newLength >= 3) {
            currentLength = newLength;
        }
        else {
            currentLength = 3; // Keep the minimum length of 3
        }


        isPowerUpActive = false;  // Deactivate the power-up
    }
}



// Check self-collision
void SnakeGame::checkSelfCollision() {
    // Skip self-collision check for the first 2 segments behind the head
    int startIndex = (currentLength > 3) ? 2 : currentLength; 

    for (int i = startIndex; i < currentLength; ++i) {
        if (snake[0].getPosition() == snake[i].getPosition()) {
            cout << "Self-collision detected!" << endl;
            isGameOver = true;
            return;
        }
    }
}


// Check boundary collision
void SnakeGame::checkBorderCollision() {
    Point head = snake[0].getPosition();
    if (head.x < 0 || head.x >= 1280 || head.y < 120 || head.y >= 720) {
        cout << "Boundary collision detected!" << endl;
        isGameOver = true;
    }
}

// Render game elements
void SnakeGame::render(sf::RenderWindow& window) {
    window.clear();

    window.draw(backgroundSprite);

    for (int i = 0; i < currentLength; ++i) {
        snake[i].draw(window);
    }

    window.draw(foodSprite);

    // Draw the power-up if active
    if (isPowerUpActive) {
        window.draw(powerUpSprite);
    }

    scoreText.setString("Score: " + to_string(score));
    window.draw(scoreText);

    // Display speed (the smaller timeStep, the faster the snake)
    float speed = 1.0f / timeStep;  // Calculate speed from timeStep
    speedText.setString("Speed: " + to_string(static_cast<int>(speed)));  // Display speed
    window.draw(speedText);


    if (isGameOver) {
        window.draw(gameOverText);
    }

    if (isPaused) {
        sf::Text pausedText;
        pausedText.setFont(font);
        pausedText.setString("Game Paused\nPress P to Resume");
        pausedText.setCharacterSize(40);
        pausedText.setFillColor(sf::Color::Yellow);
        pausedText.setPosition(400, 300);
        window.draw(pausedText);
    }

    // Draw elapsed time at the top-right of the screen
    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(30);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(1100, 20);
    timeText.setString("Time: " + to_string(static_cast<int>(elapsedTime.asSeconds())) + "s");
    window.draw(timeText);



}
