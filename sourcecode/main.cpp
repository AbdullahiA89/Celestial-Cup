// ---------------- Includes ----------------
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iostream>
#include <cmath> // For std::sin



// ---------------- Main Function ----------------
int main() {
    // ---------------- Initialize Window ----------------
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Celestial Cup", sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);


    // ---------------- Enums and State ----------------
    enum GameState {
        MainMenu,
        Playing,
        Settings
    };
    GameState currentGameState = MainMenu;

    // Load Main Menu Background
    sf::Texture mainMenuBgTexture;

    // ---------------- Resource Loading ----------------
    if (!mainMenuBgTexture.loadFromFile("bin/debug/res/mainmenubg.png")) {
        std::cerr << "Failed to load main menu background!" << std::endl;
        return -1;
    }
    sf::Sprite mainMenuBg(mainMenuBgTexture);

    // Load Font
    sf::Font font;
    if (!font.loadFromFile("bin/debug/res/font/Pixelify_Sans/PixelifySans-VariableFont_wght.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Declare Main Menu Elements
    sf::Text titleText, startText, settingsText, quitText;
    sf::RectangleShape startBox, settingsBox, quitBox;

    // Initialize Title Text
    titleText.setFont(font);
    titleText.setString("Celestial Cup");
    titleText.setCharacterSize(100);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(960 - titleText.getGlobalBounds().width / 2, 400);

    // Initialize Menu Options
    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);

    settingsText.setFont(font);
    settingsText.setString("Settings");
    settingsText.setCharacterSize(40);
    settingsText.setFillColor(sf::Color::White);

    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(40);
    quitText.setFillColor(sf::Color::White);

    // Position Text Inside Boxes
    startBox.setSize(sf::Vector2f(300, 70));
    startBox.setFillColor(sf::Color(100, 100, 100, 200)); // Semi-transparent gray
    startBox.setOutlineColor(sf::Color::White);
    startBox.setOutlineThickness(2);
    startBox.setPosition(810, 600); // Bottom middle
    startText.setPosition(
        startBox.getPosition().x + startBox.getSize().x / 2 - startText.getGlobalBounds().width / 2,
        startBox.getPosition().y + startBox.getSize().y / 2 - startText.getGlobalBounds().height / 2);

    settingsBox.setSize(sf::Vector2f(300, 70));
    settingsBox.setFillColor(sf::Color(100, 100, 100, 200));
    settingsBox.setOutlineColor(sf::Color::White);
    settingsBox.setOutlineThickness(2);
    settingsBox.setPosition(810, 700); // Below Start Game
    settingsText.setPosition(
        settingsBox.getPosition().x + settingsBox.getSize().x / 2 - settingsText.getGlobalBounds().width / 2,
        settingsBox.getPosition().y + settingsBox.getSize().y / 2 - settingsText.getGlobalBounds().height / 2);

    quitBox.setSize(sf::Vector2f(300, 70));
    quitBox.setFillColor(sf::Color(100, 100, 100, 200));
    quitBox.setOutlineColor(sf::Color::White);
    quitBox.setOutlineThickness(2);
    quitBox.setPosition(810, 800); // Below Settings
    quitText.setPosition(
        quitBox.getPosition().x + quitBox.getSize().x / 2 - quitText.getGlobalBounds().width / 2,
        quitBox.getPosition().y + quitBox.getSize().y / 2 - quitText.getGlobalBounds().height / 2);


    // ---------------- Game Objects ----------------
    sf::RectangleShape field(sf::Vector2f(1920.0f, 1080.0f));
    field.setFillColor(sf::Color(50, 150, 50)); // Green grass

    sf::RectangleShape player1(sf::Vector2f(65.0f, 125.0f));
    player1.setFillColor(sf::Color::Blue);
    sf::Vector2f player1StartPos(200.0f, 880.0f);
    player1.setPosition(player1StartPos);

    sf::RectangleShape player2(sf::Vector2f(65.0f, 125.0f));
    player2.setFillColor(sf::Color::Red);
    sf::Vector2f player2StartPos(1670.0f, 880.0f);
    player2.setPosition(player2StartPos);

    sf::RectangleShape player1Foot(sf::Vector2f(140.0f, 20.0f)); // Rectangular foot
    player1Foot.setFillColor(sf::Color::Cyan);

    sf::RectangleShape player2Foot(sf::Vector2f(140.0f, 20.0f)); // Rectangular foot
    player2Foot.setFillColor(sf::Color::Magenta);

    // Initialize Player 1's foot position
    player1Foot.setOrigin(player1Foot.getSize().x / 2.0f, player1Foot.getSize().y / 2.0f); // Set origin to the center
    player1Foot.setPosition(player1StartPos.x + player1.getSize().x / 2.0f, player1StartPos.y + player1.getSize().y + 20.0f); // Position below Player 1

    // Initialize Player 2's foot position
    player2Foot.setOrigin(player2Foot.getSize().x / 2.0f, player2Foot.getSize().y / 2.0f); // Set origin to the center
    player2Foot.setPosition(player2StartPos.x + player2.getSize().x / 2.0f, player2StartPos.y + player2.getSize().y + 20.0f); // Position below Player 2

    sf::CircleShape ball(25.0f);
    ball.setFillColor(sf::Color::White);
    sf::Vector2f ballStartPos(945.0f, 540.0f);
    ball.setPosition(ballStartPos);

    // Define resolutions
    std::vector<sf::Vector2u> resolutions = {
        {1920, 1080}, // Full HD
        {1600, 900},  // HD+
        {1366, 768},  // WXGA
        {1280, 720},  // HD
        {1024, 576}   // SD
    };

    int currentResolutionIndex = 0; // Start with the first resolution

    sf::Text resolutionText, resolutionLeftArrow, resolutionRightArrow, applyButton;

    // Initialize UI
    resolutionText.setFont(font);
    resolutionText.setCharacterSize(40);
    resolutionText.setFillColor(sf::Color::White);
    resolutionText.setPosition(800, 300); // Center the resolution display

    resolutionLeftArrow.setFont(font);
    resolutionLeftArrow.setString("<");
    resolutionLeftArrow.setCharacterSize(40);
    resolutionLeftArrow.setFillColor(sf::Color::White);
    resolutionLeftArrow.setPosition(750, 300); // Left arrow position

    resolutionRightArrow.setFont(font);
    resolutionRightArrow.setString(">");
    resolutionRightArrow.setCharacterSize(40);
    resolutionRightArrow.setFillColor(sf::Color::White);
    resolutionRightArrow.setPosition(1000, 300); // Right arrow position

    applyButton.setFont(font);
    applyButton.setString("Apply");
    applyButton.setCharacterSize(40);
    applyButton.setFillColor(sf::Color::White);
    applyButton.setPosition(850, 400); // Position for "Apply" button


    // ---------------- Physics Variables ----------------
    sf::Vector2f ballVelocity(0.0f, 0.0f); // Ball's velocity (x, y)
    const float ballFriction = 0.70f; // Friction for slowing ball horizontally
    const float ballBounce = -0.7f; // Ball bounces with reduced velocity
    const float kickStrength = 10.0f; // Strength of the kick

    // ---------------- Goals ----------------
    sf::RectangleShape leftGoal(sf::Vector2f(50.0f, 200.0f));
    leftGoal.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
    leftGoal.setPosition(0.0f, 680.0f);

    sf::RectangleShape rightGoal(sf::Vector2f(50.0f, 200.0f));
    rightGoal.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
    rightGoal.setPosition(1870.0f, 680.0f);

    // ---------------- Game Mechanics Variables ----------------
    // Scores
    int scorePlayer1 = 0;
    int scorePlayer2 = 0;

    // Jump variables
    float player1VelocityY = 0.0f;
    float player2VelocityY = 0.0f;
    const float jumpStrength = -15.0f;
    const float gravity = 0.5f;

    bool player1OnGround = true;
    bool player2OnGround = true;

    // Timer for resetting positions after a goal
    sf::Clock resetClock;
    bool isResetting = true; // Game starts in reset state

    // Swing angles for feet
    float player1FootAngle = 0.0f;
    float player2FootAngle = 0.0f;

    // Swing states
    bool player1Swinging = false;
    bool player2Swinging = false;

    bool player1CanDoubleJump = false; // Tracks if Player 1 can perform a double jump



    // ---------------- Load Textures ----------------
    // Load Textures
    sf::Texture backgroundTexture, ballTexture, leftGoalTexture, rightGoalTexture, player1Texture, player2Texture, player1FootTexture, player2FootTexture;
    if (!backgroundTexture.loadFromFile("bin/debug/res/Spacefinal.png") ||
        !ballTexture.loadFromFile("bin/debug/res/ball.png") ||
        !leftGoalTexture.loadFromFile("bin/debug/res/leftgoalpost.png") ||
        !rightGoalTexture.loadFromFile("bin/debug/res/rightgoalpost.png") ||
        !player1Texture.loadFromFile("bin/debug/res/Player1.png") ||
        !player2Texture.loadFromFile("bin/debug/res/Player2.png") ||
        !player1FootTexture.loadFromFile("bin/debug/res/player1shoe.png") ||
        !player2FootTexture.loadFromFile("bin/debug/res/player2shoe.png")) {
        std::cerr << "Failed to load one or more textures!" << std::endl;
        return -1;
    }

    // Create Sprites
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Sprite ballSprite(ballTexture);
    sf::Sprite leftGoalSprite(leftGoalTexture);
    sf::Sprite rightGoalSprite(rightGoalTexture);
    sf::Sprite player1Sprite(player1Texture);
    sf::Sprite player2Sprite(player2Texture);
    sf::Sprite player1FootSprite(player1FootTexture);
    sf::Sprite player2FootSprite(player2FootTexture);

    // Adjust boot size
    player1FootSprite.setScale(0.2f, 0.2f); // Adjust size for Player 1's boot
    player2FootSprite.setScale(0.2f, 0.2f); // Adjust size for Player 2's boot

    // Set background position
    backgroundSprite.setPosition(0.0f, 0.0f); // Fullscreen background

    sf::Text scoreboardText;

    // Initialize the Scoreboard Text
    scoreboardText.setFont(font);
    scoreboardText.setCharacterSize(50); // Large font size for visibility
    scoreboardText.setFillColor(sf::Color::White); // White text
    scoreboardText.setOutlineColor(sf::Color::Black); // Black outline for contrast
    scoreboardText.setOutlineThickness(3.0f); // Thick outline for visibility
    scoreboardText.setString("Player 1: 0  -  Player 2: 0"); // Initial score
    scoreboardText.setPosition(960 - scoreboardText.getGlobalBounds().width / 2, 20); // Centered at the top


    // Player 1 Abilities
    bool player1SpeedBoost = false;
    sf::Clock player1SpeedBoostTimer;
    sf::Text player1SpeedBoostText;

    // Player 2 Abilities
    bool player2GiantJuice = false;
    sf::Clock player2GiantJuiceTimer;
    sf::Text player2GiantJuiceText;

    // Player 1 Speed Boost Text
    player1SpeedBoostText.setFont(font);
    player1SpeedBoostText.setCharacterSize(40);
    player1SpeedBoostText.setFillColor(sf::Color::Black); // Text color
    player1SpeedBoostText.setOutlineColor(sf::Color::Yellow); // Outline color
    player1SpeedBoostText.setOutlineThickness(3.0f); // Outline thickness
    player1SpeedBoostText.setPosition(50, 50); // Left side


    // Player 2 Giant Juice Text
    player2GiantJuiceText.setFont(font);
    player2GiantJuiceText.setCharacterSize(40);
    player2GiantJuiceText.setFillColor(sf::Color::Blue); // Text color
    player2GiantJuiceText.setOutlineColor(sf::Color::Red); // Outline color
    player2GiantJuiceText.setOutlineThickness(3.0f); // Outline thickness
    player2GiantJuiceText.setPosition(1600, 50);


    sf::Clock gameClock; // Clock to track the 2-minute timer
    float gameDuration = 120.0f; // Total game duration in seconds (2 minutes)

    // Game Over flag
    bool isGameOver = false;
    std::string winnerText;

    // Victory Screen Text
    sf::Text victoryText;

    sf::RectangleShape mainMenuButton;

    sf::Text mainMenuButtonText;
    // Initialize Main Menu Button
    mainMenuButton.setSize(sf::Vector2f(300, 70)); // Button size
    mainMenuButton.setFillColor(sf::Color(100, 100, 100, 200)); // Semi-transparent gray
    mainMenuButton.setOutlineColor(sf::Color::White); // White outline
    mainMenuButton.setOutlineThickness(2.0f); // Outline thickness
    mainMenuButton.setPosition(810, 850); // Center bottom

    // Initialize Main Menu Button Text
    mainMenuButtonText.setFont(font);
    mainMenuButtonText.setString("Main Menu");
    mainMenuButtonText.setCharacterSize(40); // Font size
    mainMenuButtonText.setFillColor(sf::Color::White); // White text
    mainMenuButtonText.setPosition(
        mainMenuButton.getPosition().x + mainMenuButton.getSize().x / 2 - mainMenuButtonText.getGlobalBounds().width / 2,
        mainMenuButton.getPosition().y + mainMenuButton.getSize().y / 2 - mainMenuButtonText.getGlobalBounds().height / 2
    ); // Center text in button



    // Initialize Victory Screen Text
    victoryText.setFont(font);
    victoryText.setCharacterSize(100); // Large size
    victoryText.setFillColor(sf::Color::White); // White text
    victoryText.setOutlineColor(sf::Color::Black); // Black outline
    victoryText.setOutlineThickness(3.0f); // Thick outline for visibility
    victoryText.setString(""); // Initially empty
    victoryText.setPosition(960 - victoryText.getGlobalBounds().width / 2, 540 - victoryText.getGlobalBounds().height / 2); // Center


    sf::Text settingsTitleText;
    settingsTitleText.setFont(font);
    settingsTitleText.setString("Settings");
    settingsTitleText.setCharacterSize(80);
    settingsTitleText.setFillColor(sf::Color::White);
    settingsTitleText.setPosition(960 - settingsTitleText.getGlobalBounds().width / 2, 200);

    sf::Text backButton;

    // Initialize Back Button
    backButton.setFont(font);
    backButton.setString("Back to Main Menu");
    backButton.setCharacterSize(40);
    backButton.setFillColor(sf::Color::White);
    backButton.setPosition(800, 500); // Position it appropriately in the Settings menu

    bool isPaused = false; // Pause state

    sf::Text resumeButton, pausemainMenuButton;

    // Initialize Pause Menu Buttons
    resumeButton.setFont(font);
    resumeButton.setString("Resume");
    resumeButton.setCharacterSize(50);
    resumeButton.setFillColor(sf::Color::White);
    resumeButton.setPosition(800, 400); // Adjust based on resolution

    pausemainMenuButton.setFont(font);
    pausemainMenuButton.setString("Main Menu");
    pausemainMenuButton.setCharacterSize(50);
    pausemainMenuButton.setFillColor(sf::Color::White);
    pausemainMenuButton.setPosition(800, 500); // Adjust based on resolution


    // ---------------- Main Game Loop ----------------
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }


            // ---------------- Enums and State ----------------
            if (currentGameState == MainMenu) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (startText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        // Reset timer and score
                        scorePlayer1 = 0;
                        scorePlayer2 = 0;
                        gameClock.restart();
                        scoreboardText.setString("Player 1: 0  -  Player 2: 0  |  Time: 2:00");
                        scoreboardText.setPosition(960 - scoreboardText.getGlobalBounds().width / 2, 20);
                        currentGameState = Playing;
                    }
                    else if (settingsText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentGameState = Settings;
                    }
                    else if (quitText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        window.close();
                    }
                }
            }
            else if (currentGameState == Settings) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Navigate resolutions
                    if (resolutionLeftArrow.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentResolutionIndex = (currentResolutionIndex - 1 + resolutions.size()) % resolutions.size(); // Loop back if needed
                    }
                    else if (resolutionRightArrow.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentResolutionIndex = (currentResolutionIndex + 1) % resolutions.size(); // Loop forward if needed
                    }

                    // Apply resolution
                    if (applyButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        // Get the selected resolution
                        sf::Vector2u selectedResolution = resolutions[currentResolutionIndex];

                        // Create the window with the new resolution
                        window.create(sf::VideoMode(selectedResolution.x, selectedResolution.y), "Celestial Cup", sf::Style::Close | sf::Style::Resize);

                        // Reset the view to match the new resolution
                        sf::View newView(sf::FloatRect(0, 0, static_cast<float>(selectedResolution.x), static_cast<float>(selectedResolution.y)));
                        window.setView(newView);
                    }

                    // Back to Main Menu
                    if (backButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentGameState = MainMenu; // Return to main menu
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                isPaused = !isPaused; // Toggle pause state
            }

        }

        // Pause Menu Logic
        if (isPaused) {
            window.clear(sf::Color(0, 0, 0));
            window.draw(resumeButton);
            window.draw(pausemainMenuButton);
            window.display();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Resume Button
                if (resumeButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    isPaused = false;
                }

                // Main Menu Button
                if (pausemainMenuButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentGameState = MainMenu;
                    isPaused = false;
                }
            }

            continue; // Skip the rest of the game loop when paused
        }


        if (!isResetting) {
            // ---------------- Player Movement ----------------
            // Player 1 Movement (WAD)
            // Player 1 Speed Boost (Key: G)
            // Speed Boost Activation (Key: G)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && player1SpeedBoostTimer.getElapsedTime().asSeconds() > 15.0f && !player1SpeedBoost) {
                player1SpeedBoost = true;
                player1SpeedBoostTimer.restart();
                player1CanDoubleJump = true; // Enable double jump when speed boost is activated
            }

            // Deactivate Speed Boost after 3 seconds
            if (player1SpeedBoost && player1SpeedBoostTimer.getElapsedTime().asSeconds() > 3.0f) {
                player1SpeedBoost = false;
                player1CanDoubleJump = false; // Disable double jump after speed boost ends
            }

            // Update Player 1's movement speed
            float player1CurrentSpeed = player1SpeedBoost ? 10.0f : 5.0f;


            // Player 1 Movement with updated speed
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.getPosition().x > 0)
                player1.move(-player1CurrentSpeed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.getPosition().x + player1.getSize().x < 1920)
                player1.move(player1CurrentSpeed, 0);

            // Update cooldown text
            player1SpeedBoostText.setString("Speed Boost: " + std::to_string((int)std::max(0.0f, 15.0f - player1SpeedBoostTimer.getElapsedTime().asSeconds())) + "s");

            // Player 1 Jump Logic (W Key)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if (player1OnGround) {
                    player1VelocityY = jumpStrength; // Regular jump
                    player1OnGround = false;
                }
                else if (player1CanDoubleJump) {
                    player1VelocityY = jumpStrength; // Double jump
                    player1CanDoubleJump = false;    // Disable double jump after use
                }
            }


            // Player 2 Movement (Arrow Keys)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player2.getPosition().x > 0)
                player2.move(-5.0f, 0); // Move left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player2.getPosition().x + player2.getSize().x < 1920)
                player2.move(5.0f, 0); // Move right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player2OnGround) {
                player2VelocityY = jumpStrength; // Jump
                player2OnGround = false;
            }

            // ---------------- Gravity ----------------
            player1VelocityY += gravity; // Gravity affects Player 1
            player2VelocityY += gravity; // Gravity affects Player 2

            // Update vertical positions
            player1.move(0, player1VelocityY);
            player2.move(0, player2VelocityY);

            // ---------------- Ground Collision ----------------
            if (player1.getPosition().y + player1.getSize().y >= 880) {
                player1.setPosition(player1.getPosition().x, 880 - player1.getSize().y);
                player1VelocityY = 0.0f;
                player1OnGround = true;

                // Reset double jump if speed boost is active
                if (player1SpeedBoost) {
                    player1CanDoubleJump = true;
                }
            }


            if (player2.getPosition().y + player2.getSize().y >= 880) {
                player2.setPosition(player2.getPosition().x, 880 - player2.getSize().y);
                player2VelocityY = 0.0f;
                player2OnGround = true;
            }

            // ---------------- Ball Physics ----------------
            ballVelocity.y += gravity; // Apply gravity to the ball
            ball.move(ballVelocity);

            // Ball Collision with ground
            if (ball.getPosition().y + ball.getRadius() * 2 >= 880) {
                ball.setPosition(ball.getPosition().x, 880 - ball.getRadius() * 2);
                ballVelocity.y *= ballBounce; // Reverse and reduce vertical velocity for bounce

                // Apply horizontal ground friction
                ballVelocity.x *= ballFriction;

                // Stop horizontal movement if velocity is very low
                if (std::abs(ballVelocity.x) < 0.1f) {
                    ballVelocity.x = 0.0f;
                }
            }



            // Ball Collision with walls
            if (ball.getPosition().x <= 0 || ball.getPosition().x + ball.getRadius() * 2 >= 1920) {
                ballVelocity.x *= -1; // Reverse horizontal velocity
                if (ball.getPosition().x <= 0)
                    ball.setPosition(0, ball.getPosition().y);
                if (ball.getPosition().x + ball.getRadius() * 2 >= 1920)
                    ball.setPosition(1920 - ball.getRadius() * 2, ball.getPosition().y);
            }

            // Constrain ball within the vertical bounds of the window
            if (ball.getPosition().y <= 0) { // Ball hits the top of the window
                ball.setPosition(ball.getPosition().x, 0); // Reset to top edge
                ballVelocity.y *= -1; // Reverse vertical velocity
            }

            if (ball.getPosition().y + ball.getRadius() * 2 >= 1080) { // Ball hits the bottom of the window
                ball.setPosition(ball.getPosition().x, 1080 - ball.getRadius() * 2); // Reset to bottom edge
                ballVelocity.y *= ballBounce; // Reverse and reduce vertical velocity for bounce
            }


            // Ball Collision with players
            sf::FloatRect ballBounds = ball.getGlobalBounds();
            sf::FloatRect player1Bounds = player1.getGlobalBounds();
            sf::FloatRect player2Bounds = player2.getGlobalBounds();

            // Ball Collision with players' feet (during swings)
            sf::FloatRect player1FootBounds = player1Foot.getGlobalBounds();
            sf::FloatRect player2FootBounds = player2Foot.getGlobalBounds();

            // Handle collision with Player 1's body
            if (ballBounds.intersects(player1Bounds)) {
                if (ball.getPosition().x < player1.getPosition().x + player1.getSize().x / 2) {
                    // Ball is on the left side, push it to the left
                    ball.setPosition(player1.getPosition().x - ball.getRadius() * 2, ball.getPosition().y);
                    ballVelocity.x = -std::abs(ballVelocity.x); // Ensure it moves to the left
                }
                else {
                    // Ball is on the right side, push it to the right
                    ball.setPosition(player1.getPosition().x + player1.getSize().x, ball.getPosition().y);
                    ballVelocity.x = std::abs(ballVelocity.x); // Ensure it moves to the right
                }
                ballVelocity.y *= 0.7f; // Dampen vertical velocity slightly
            }

            // Handle collision with Player 2's body
            if (ballBounds.intersects(player2Bounds)) {
                if (ball.getPosition().x < player2.getPosition().x + player2.getSize().x / 2) {
                    // Ball is on the left side, push it to the left
                    ball.setPosition(player2.getPosition().x - ball.getRadius() * 2, ball.getPosition().y);
                    ballVelocity.x = -std::abs(ballVelocity.x); // Ensure it moves to the left
                }
                else {
                    // Ball is on the right side, push it to the right
                    ball.setPosition(player2.getPosition().x + player2.getSize().x, ball.getPosition().y);
                    ballVelocity.x = std::abs(ballVelocity.x); // Ensure it moves to the right
                }
                ballVelocity.y *= 0.7f; // Dampen vertical velocity slightly
            }

            // Handle collision with Player 1's foot during swing
            if (player1FootBounds.intersects(ballBounds) && player1Swinging) {
                ballVelocity.x = 15.0f * std::cos(player1FootAngle * 3.14f / 180.0f); // Apply horizontal force
                ballVelocity.y = -10.0f; // Apply vertical force
                ball.setPosition(player1Foot.getPosition().x, player1Foot.getPosition().y - ball.getRadius() * 2); // Position above foot
            }

            // Handle collision with Player 2's foot during swing
            if (player2FootBounds.intersects(ballBounds) && player2Swinging) {
                ballVelocity.x = -15.0f * std::cos(player2FootAngle * 3.14f / 180.0f); // Apply horizontal force
                ballVelocity.y = -10.0f; // Apply vertical force
                ball.setPosition(player2Foot.getPosition().x, player2Foot.getPosition().y - ball.getRadius() * 2); // Position above foot
            }




            // Game Timer Logic
            float timeRemaining = gameDuration - gameClock.getElapsedTime().asSeconds();

            if (!isGameOver) {
                // Check if a player scores 5 goals
                if (scorePlayer1 >= 5) {
                    winnerText = "Player 1 Wins!";
                    isGameOver = true;
                }
                else if (scorePlayer2 >= 5) {
                    winnerText = "Player 2 Wins!";
                    isGameOver = true;
                }

                // Check if the timer runs out
                if (timeRemaining <= 0) {
                    if (scorePlayer1 > scorePlayer2) {
                        winnerText = "Player 1 Wins!";
                    }
                    else if (scorePlayer2 > scorePlayer1) {
                        winnerText = "Player 2 Wins!";
                    }
                    else {
                        winnerText = "It's a Tie!";
                    }
                    isGameOver = true;
                }

                // Update the scoreboard with the timer
                int minutes = static_cast<int>(timeRemaining) / 60;
                int seconds = static_cast<int>(timeRemaining) % 60;
                scoreboardText.setString(
                    "Player 1: " + std::to_string(scorePlayer1) +
                    "  -  Player 2: " + std::to_string(scorePlayer2) +
                    "  |  Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds)
                );
                scoreboardText.setPosition(960 - scoreboardText.getGlobalBounds().width / 2, 20); // Center
            }

            if (isGameOver) {
                victoryText.setString(winnerText);
                victoryText.setPosition(960 - victoryText.getGlobalBounds().width / 2, 540 - victoryText.getGlobalBounds().height / 2); // Center Victory Text

                // Check if 5 seconds have passed
                if (gameClock.getElapsedTime().asSeconds() > gameDuration + 5.0f) {
                    // Automatically return to Main Menu after 5 seconds
                    currentGameState = MainMenu;
                    isGameOver = false;
                    gameClock.restart();
                    scorePlayer1 = 0;
                    scorePlayer2 = 0;

                    // Reset scoreboard
                    scoreboardText.setString("Player 1: 0  -  Player 2: 0  |  Time: 2:00");
                    scoreboardText.setPosition(960 - scoreboardText.getGlobalBounds().width / 2, 20);
                }

                // Handle "Main Menu" button click
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (mainMenuButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        // Reset the game state and return to the main menu
                        currentGameState = MainMenu;
                        isGameOver = false;
                        gameClock.restart();
                        scorePlayer1 = 0;
                        scorePlayer2 = 0;

                        // Reset scoreboard
                        scoreboardText.setString("Player 1: 0  -  Player 2: 0  |  Time: 2:00");
                        scoreboardText.setPosition(960 - scoreboardText.getGlobalBounds().width / 2, 20);
                    }
                }
            }



            // ---------------- Dash Mechanics ----------------
            const float dashSpeed = 20.0f; // Speed of the dash
            const float dashDuration = 0.2f; // Duration of the dash in seconds
            const float dashCooldown = 1.0f; // Cooldown time in seconds

            // Player 1 Dash Variables
            static sf::Clock player1DashClock;
            static sf::Clock player1DashCooldownClock;
            static bool player1Dashing = false;

            // Player 2 Dash Variables
            static sf::Clock player2DashClock;
            static sf::Clock player2DashCooldownClock;
            static bool player2Dashing = false;

            // Player 1 Dash (Shift Key)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::V) && player1DashCooldownClock.getElapsedTime().asSeconds() > dashCooldown && !player1Dashing) {
                player1Dashing = true;
                player1DashClock.restart(); // Start dash timer
                player1DashCooldownClock.restart(); // Start cooldown timer
            }

            if (player1Dashing) {
                if (player1DashClock.getElapsedTime().asSeconds() < dashDuration) {
                    player1.move((sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? -dashSpeed : dashSpeed), 0); // Dash left or right
                }
                else {
                    player1Dashing = false; // End dash
                }
            }

            // Player 2 Dash (Ctrl Key)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && player2DashCooldownClock.getElapsedTime().asSeconds() > dashCooldown && !player2Dashing) {
                player2Dashing = true;
                player2DashClock.restart(); // Start dash timer
                player2DashCooldownClock.restart(); // Start cooldown timer
            }

            if (player2Dashing) {
                if (player2DashClock.getElapsedTime().asSeconds() < dashDuration) {
                    player2.move((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? -dashSpeed : dashSpeed), 0); // Dash left or right
                }
                else {
                    player2Dashing = false; // End dash
                }
            }


            // Player 2 Giant Juice (Key: L)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && player2GiantJuiceTimer.getElapsedTime().asSeconds() > 15.0f && !player2GiantJuice) {
                player2GiantJuice = true;
                player2GiantJuiceTimer.restart();

                // Update Player 2 size
                player2.setSize(sf::Vector2f(200.0f, 400.0f)); // Bigger size for noticeable effect
                player2.setPosition(player2.getPosition().x, player2.getPosition().y - 275.0f); // Adjust position to prevent sinking into the ground

                // Update Player 2 sprite scale
                player2Sprite.setScale(3.0f, 3.0f); // Enlarge the sprite
            }

            // Revert size after 4 seconds
            if (player2GiantJuice && player2GiantJuiceTimer.getElapsedTime().asSeconds() > 3.0f) {
                player2.setSize(sf::Vector2f(65.0f, 125.0f)); // Revert to original size
                player2GiantJuice = false;

                // Reset Player 2 sprite scale
                player2Sprite.setScale(1.0f, 1.0f); // Reset to normal size
            }



            // Update cooldown text
            player2GiantJuiceText.setString("Giant Juice: " + std::to_string((int)std::max(0.0f, 15.0f - player2GiantJuiceTimer.getElapsedTime().asSeconds())) + "s");

            // ---------------- Swing Mechanics ----------------
            // Swing Player 1 Foot (B Key)
            static float player1SwingCharge = 0.0f; // Charge for the swing
            static float player1SwingSpeed = 0.0f;  // Speed for the forward motion

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                player1Swinging = true;
                player1SwingCharge -= 5.0f; // Increase swing angle (negative for counter-clockwise)
                player1SwingCharge = std::max(player1SwingCharge, -75.0f); // Limit backward swing
                player1FootSprite.setRotation(player1SwingCharge); // Rotate backward
            }
            else if (player1Swinging) {
                player1SwingSpeed += 5.0f; // Forward swing speed
                player1SwingCharge += player1SwingSpeed * 0.1f + 5.0f; // Forward fling motion
                player1FootSprite.setRotation(player1SwingCharge); // Rotate forward
                if (player1SwingCharge >= 0.0f) { // Reset foot position
                    player1SwingSpeed = 0.0f;
                    player1Swinging = false;
                    player1SwingCharge = 0.0f;
                    player1FootSprite.setRotation(0.0f); // Reset rotation
                }
            }

            // Swing Player 2 Foot (P Key)
            static float player2SwingCharge = 0.0f; // Charge for the swing
            static float player2SwingSpeed = 0.0f;  // Speed for the forward motion

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                player2Swinging = true;
                player2SwingCharge += 5.0f; // Increase swing angle
                player2SwingCharge = std::min(player2SwingCharge, 75.0f); // Limit backward swing
                player2FootSprite.setRotation(player2SwingCharge); // Rotate forward
            }
            else if (player2Swinging) {
                player2SwingSpeed += 5.0f; // Forward swing speed
                player2SwingCharge -= player2SwingSpeed * 0.1f + 5.0f; // Forward fling motion
                player2FootSprite.setRotation(player2SwingCharge); // Rotate back
                if (player2SwingCharge <= 0.0f) { // Reset foot position
                    player2SwingSpeed = 0.0f;
                    player2Swinging = false;
                    player2SwingCharge = 0.0f;
                    player2FootSprite.setRotation(0.0f); // Reset rotation
                }
            }




            // Update Player 1's foot position and rotation
            player1FootSprite.setOrigin(0, player1FootSprite.getLocalBounds().height / 2.0f);
            player1Foot.setRotation(player1FootAngle); // Apply the swing angle
            player1Foot.setPosition(player1.getPosition().x + player1.getSize().x * 0.2f + 35.0f, player1.getPosition().y + player1.getSize().y); // Attach foot to bottom center of Player 1

            // Update Player 2's foot position and rotation (Flipped to the left side of Player 2's body)
            player2FootSprite.setOrigin(player2FootSprite.getLocalBounds().width, player2FootSprite.getLocalBounds().height / 2.0f);
            player2Foot.setRotation(player2FootAngle); // Apply the swing angle
            player2Foot.setPosition(
                player2.getPosition().x - player2Foot.getSize().x * 0.1f + 35.0f, // Align horizontally on the left side of Player 2
                player2.getPosition().y + player2.getSize().y + 5.0f // Slightly above the bottom of Player 2's body
            );


            // ---------------- Goal Check ----------------
            // Check if the ball enters the left goal
            if (ball.getPosition().x <= leftGoal.getPosition().x + leftGoal.getSize().x &&
                ball.getPosition().y + ball.getRadius() * 2 >= leftGoal.getPosition().y &&
                ball.getPosition().y <= leftGoal.getPosition().y + leftGoal.getSize().y) {
                scorePlayer2++; // Player 2 scores
                isResetting = true;
                resetClock.restart();

                // Update scoreboard text
                scoreboardText.setString("Player 1: " + std::to_string(scorePlayer1) + "  -  Player 2: " + std::to_string(scorePlayer2));
                scoreboardText.setPosition(960 - scoreboardText.getGlobalBounds().width / 2, 20); // Re-center after update
            }

            // Check if the ball enters the right goal
            if (ball.getPosition().x + ball.getRadius() * 2 >= rightGoal.getPosition().x &&
                ball.getPosition().y + ball.getRadius() * 2 >= rightGoal.getPosition().y &&
                ball.getPosition().y <= rightGoal.getPosition().y + rightGoal.getSize().y) {
                scorePlayer1++; // Player 1 scores
                isResetting = true;
                resetClock.restart();

                // Update scoreboard text
                scoreboardText.setString("Player 1: " + std::to_string(scorePlayer1) + "  -  Player 2: " + std::to_string(scorePlayer2));
                scoreboardText.setPosition(960 - scoreboardText.getGlobalBounds().width / 2, 20); // Re-center after update
            }
        }

        else {
            // ---------------- Reset State ----------------
            if (resetClock.getElapsedTime().asSeconds() >= 3) {
                isResetting = false;
                player1.setPosition(player1StartPos);
                player2.setPosition(player2StartPos);
                ball.setPosition(ballStartPos);
                ballVelocity = sf::Vector2f(0.0f, 0.0f);
            }
        }

        // ---------------- Rendering ----------------
        window.clear();

        // Check the current game state and render accordingly
        if (currentGameState == MainMenu) {
            // Render Main Menu
            window.draw(mainMenuBg); // Draw the background
            window.draw(titleText);  // Draw the title
            window.draw(startBox);   // Draw the box for "Start Game"
            window.draw(startText);  // Draw the text for "Start Game"
            window.draw(settingsBox); // Draw the box for "Settings"
            window.draw(settingsText); // Draw the text for "Settings"
            window.draw(quitBox);    // Draw the box for "Quit"
            window.draw(quitText);   // Draw the text for "Quit"

        }
        else if (currentGameState == Playing) {
            // Render Game Scene

            // Draw the background
            window.draw(backgroundSprite);

            // Sync goal sprites with their respective positions
            leftGoalSprite.setPosition(leftGoal.getPosition());
            rightGoalSprite.setPosition(rightGoal.getPosition());

            // Draw goals
            window.draw(leftGoalSprite);
            window.draw(rightGoalSprite);

            // Sync ball sprite with its position
            ballSprite.setPosition(ball.getPosition());

            // Draw the ball
            window.draw(ballSprite);

            player1Sprite.setPosition(player1.getPosition());
            window.draw(player1Sprite);

            player2Sprite.setPosition(player2.getPosition());
            window.draw(player2Sprite);

            player1FootSprite.setPosition(player1Foot.getPosition());
            window.draw(player1FootSprite);

            player2FootSprite.setPosition(player2Foot.getPosition());
            window.draw(player2FootSprite);

            window.draw(player1SpeedBoostText);
            window.draw(player2GiantJuiceText);

            if (!isGameOver) {
                // Render the scoreboard
                window.draw(scoreboardText);
            }
            else {
                // Render the victory screen
                window.draw(victoryText);
                // Render Main Menu Button
                window.draw(mainMenuButton);
                window.draw(mainMenuButtonText);

            }

        }

        else if (currentGameState == Settings) {

            // Update resolution text
            sf::Vector2u currentResolution = resolutions[currentResolutionIndex];
            resolutionText.setString(std::to_string(currentResolution.x) + " x " + std::to_string(currentResolution.y));

            // Render UI elements
            window.draw(resolutionLeftArrow);
            window.draw(resolutionText);
            window.draw(resolutionRightArrow);
            window.draw(applyButton);

            window.draw(backButton);
        }


        // Display the rendered frame
        window.display();
    }

    return 0;
}
