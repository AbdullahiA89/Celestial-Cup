// ---------------- Includes ----------------
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
<<<<<<< Updated upstream
<<<<<<< HEAD
=======
#include <cmath>
>>>>>>> a49d4a4427e7bc0b22c8b09d2cb7706bde024777
=======
#include <cmath>
>>>>>>> Stashed changes

// ---------------- Main Function ----------------
enum GameState {
    MainMenu,
    Playing
};

int main() {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Celestial Cup");
    window.setFramerateLimit(60);

    // Load font for menu text
    sf::Font font;
<<<<<<< Updated upstream
    if (!font.loadFromFile("bin/debug/res/Pixelify_Sans/PixelifySans-VariableFont_wght.ttf")) {
=======
    if (!font.loadFromFile("bin/debug/res/font/Pixelify_Sans/PixelifySans-VariableFont_wght.ttf")) {
>>>>>>> Stashed changes
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Main Menu Text Elements
    sf::Text titleText, startText, quitText;

    // Title
    titleText.setFont(font);
    titleText.setString("Celestial Cup");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(400 - titleText.getGlobalBounds().width / 2, 100);

    // Start Game Option
    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(400 - startText.getGlobalBounds().width / 2, 250);

    // Quit Option
    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(30);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(400 - quitText.getGlobalBounds().width / 2, 350);

    // Game State
    GameState currentGameState = MainMenu;

    // Gameplay Elements
    sf::RectangleShape field(sf::Vector2f(800.0f, 600.0f));
    field.setFillColor(sf::Color(50, 150, 50)); // Green grass
    field.setPosition(0, 0);

    sf::RectangleShape player1(sf::Vector2f(50.0f, 100.0f));
    player1.setFillColor(sf::Color::Blue);
    player1.setPosition(100.0f, 450.0f);

    sf::RectangleShape player2(sf::Vector2f(50.0f, 100.0f));
    player2.setFillColor(sf::Color::Red);
    player2.setPosition(650.0f, 450.0f);

<<<<<<< Updated upstream
<<<<<<< HEAD
=======
=======
>>>>>>> Stashed changes
    sf::CircleShape ball(20.0f);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(390.0f, 290.0f);

<<<<<<< Updated upstream
    // Physics Variables
    sf::Vector2f ballVelocity(0.0f, 0.0f);
    const float gravity = 0.5f;
    const float friction = 0.98f; // Slow down ball movement slightly
    const float bounce = -0.7f;   // Ball loses some energy on bounce

>>>>>>> a49d4a4427e7bc0b22c8b09d2cb7706bde024777
=======
    // Goals
    sf::RectangleShape leftGoal(sf::Vector2f(50.0f, 200.0f));
    leftGoal.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
    leftGoal.setPosition(0.0f, 400.0f);

    sf::RectangleShape rightGoal(sf::Vector2f(50.0f, 200.0f));
    rightGoal.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
    rightGoal.setPosition(750.0f, 400.0f);

    // Scores
    int scorePlayer1 = 0;
    int scorePlayer2 = 0;

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Player 1: 0 - Player 2: 0");
    scoreText.setPosition(200, 10);

    // Physics Variables
    sf::Vector2f ballVelocity(0.0f, 0.0f);
    const float gravity = 0.5f;
    const float friction = 0.98f;
    const float bounce = -0.7f;

>>>>>>> Stashed changes
    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle main menu input
            if (currentGameState == MainMenu) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (startText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentGameState = Playing; // Transition to gameplay
                    }
                    else if (quitText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        window.close();
                    }
                }
            }
        }

<<<<<<< Updated upstream
<<<<<<< HEAD
        // Render based on game state
        window.clear(sf::Color::Black);

=======
        if (currentGameState == Playing) {
            // Ball Physics
            ballVelocity.y += gravity; // Gravity affects the ball

            // Move the ball
            ball.move(ballVelocity);

            // Ball Collision with ground
            if (ball.getPosition().y + ball.getRadius() * 2 >= 600) {
                ball.setPosition(ball.getPosition().x, 600 - ball.getRadius() * 2); // Reset to ground
=======
        if (currentGameState == Playing) {
            // Ball Physics
            ballVelocity.y += gravity; // Gravity affects the ball
            ball.move(ballVelocity);

            // Ball Collision with ground
            if (ball.getPosition().y + ball.getRadius() * 2 >= 600) {
                ball.setPosition(ball.getPosition().x, 600 - ball.getRadius() * 2);
>>>>>>> Stashed changes
                ballVelocity.y *= bounce; // Reverse and reduce vertical velocity
                ballVelocity.x *= friction; // Apply horizontal friction
            }

            // Ball Collision with walls
            if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= 800) {
                ballVelocity.x *= -1; // Reverse horizontal velocity
                if (ball.getPosition().x <= 0)
                    ball.setPosition(0, ball.getPosition().y);
                if (ball.getPosition().x + ball.getRadius() * 2 >= 800)
                    ball.setPosition(800 - ball.getRadius() * 2, ball.getPosition().y);
            }
<<<<<<< Updated upstream
=======

            // Ball Collision with goals
            if (ball.getGlobalBounds().intersects(leftGoal.getGlobalBounds())) {
                scorePlayer2++;
                ball.setPosition(390.0f, 290.0f); // Reset ball position
                ballVelocity = { 0.0f, 0.0f };
            }
            if (ball.getGlobalBounds().intersects(rightGoal.getGlobalBounds())) {
                scorePlayer1++;
                ball.setPosition(390.0f, 290.0f); // Reset ball position
                ballVelocity = { 0.0f, 0.0f };
            }

            // Update the score display
            scoreText.setString("Player 1: " + std::to_string(scorePlayer1) + " - Player 2: " + std::to_string(scorePlayer2));

            // Player Movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.getPosition().x > 0) {
                player1.move(-5.0f, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.getPosition().x + player1.getSize().x < 800) {
                player1.move(5.0f, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player2.getPosition().x > 0) {
                player2.move(-5.0f, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player2.getPosition().x + player2.getSize().x < 800) {
                player2.move(5.0f, 0);
            }
>>>>>>> Stashed changes
        }

        // Render based on game state
        window.clear(sf::Color::Black);

<<<<<<< Updated upstream
>>>>>>> a49d4a4427e7bc0b22c8b09d2cb7706bde024777
=======
>>>>>>> Stashed changes
        if (currentGameState == MainMenu) {
            // Render Main Menu
            window.draw(titleText);
            window.draw(startText);
            window.draw(quitText);
        }
        else if (currentGameState == Playing) {
            // Render Gameplay Scene
            window.draw(field);
<<<<<<< Updated upstream
            window.draw(player1);
            window.draw(player2);
<<<<<<< HEAD
=======
            window.draw(ball);
>>>>>>> a49d4a4427e7bc0b22c8b09d2cb7706bde024777
=======
            window.draw(leftGoal);
            window.draw(rightGoal);
            window.draw(player1);
            window.draw(player2);
            window.draw(ball);
            window.draw(scoreText);
>>>>>>> Stashed changes
        }

        window.display();
    }

    return 0;
}
