#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath> // For std::sin

int main() {
    // ---------------- Initialize Window ----------------
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Celestial Cup", sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);

    enum GameState {
        MainMenu,
        Playing
    };
    GameState currentGameState = MainMenu;


    // ---------------- Game Objects ----------------
    // Field
    sf::RectangleShape field(sf::Vector2f(1920.0f, 1080.0f));
    field.setFillColor(sf::Color(50, 150, 50)); // Green grass

    // Player 1
    sf::RectangleShape player1(sf::Vector2f(65.0f, 125.0f));
    player1.setFillColor(sf::Color::Blue);
    sf::Vector2f player1StartPos(200.0f, 880.0f);
    player1.setPosition(player1StartPos);

    // Player 2
    sf::RectangleShape player2(sf::Vector2f(65.0f, 125.0f));
    player2.setFillColor(sf::Color::Red);
    sf::Vector2f player2StartPos(1670.0f, 880.0f);
    player2.setPosition(player2StartPos);

    // Player 1 Foot
    sf::RectangleShape player1Foot(sf::Vector2f(80.0f, 20.0f)); // Rectangular foot
    player1Foot.setFillColor(sf::Color::Cyan);

    // Player 2 Foot
    sf::RectangleShape player2Foot(sf::Vector2f(80.0f, 20.0f)); // Rectangular foot
    player2Foot.setFillColor(sf::Color::Magenta);

    // Initialize Player 1's foot position
    player1Foot.setOrigin(player1Foot.getSize().x / 2.0f, player1Foot.getSize().y / 2.0f); // Set origin to the center
    player1Foot.setPosition(player1StartPos.x + player1.getSize().x / 2.0f, player1StartPos.y + player1.getSize().y + 20.0f); // Position below Player 1

    // Initialize Player 2's foot position
    player2Foot.setOrigin(player2Foot.getSize().x / 2.0f, player2Foot.getSize().y / 2.0f); // Set origin to the center
    player2Foot.setPosition(player2StartPos.x + player2.getSize().x / 2.0f, player2StartPos.y + player2.getSize().y + 20.0f); // Position below Player 2


    // Ball
    sf::CircleShape ball(25.0f);
    ball.setFillColor(sf::Color::White);
    sf::Vector2f ballStartPos(945.0f, 540.0f);
    ball.setPosition(ballStartPos);

    // ---------------- Physics Variables ----------------
    sf::Vector2f ballVelocity(0.0f, 0.0f); // Ball's velocity (x, y)
    const float ballFriction = 0.70f; // Friction for slowing ball horizontally
    const float ballBounce = -0.7f; // Ball bounces with reduced velocity
    const float kickStrength = 20.0f; // Strength of the kick

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

    // ---------------- Main Game Loop ----------------
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!isResetting) {
            // ---------------- Player Movement ----------------
            // Player 1 Movement (WAD)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.getPosition().x > 0)
                player1.move(-5.0f, 0); // Move left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.getPosition().x + player1.getSize().x < 1920)
                player1.move(5.0f, 0); // Move right
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1OnGround) {
                player1VelocityY = jumpStrength; // Jump
                player1OnGround = false;
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
                ballVelocity.y *= ballBounce; // Reverse and reduce vertical velocity
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

            // Player 1 kicks the ball
            if (player1FootBounds.intersects(ballBounds) && player1Swinging) {
                ballVelocity.x = kickStrength * std::cos(player1FootAngle * 3.14f / 180.0f); // Apply horizontal force based on foot angle
                ballVelocity.y = kickStrength * std::sin(player1FootAngle * 3.14f / 180.0f); // Apply vertical force based on foot angle
            }

            // Player 2 kicks the ball
            if (player2FootBounds.intersects(ballBounds) && player2Swinging) {
                ballVelocity.x = -kickStrength * std::cos(player2FootAngle * 3.14f / 180.0f); // Negative for leftward direction
                ballVelocity.y = -kickStrength * std::sin(player2FootAngle * 3.14f / 180.0f); // Negative for upward force
            }

            // Prevent players from moving through each other
            if (ballBounds.intersects(player1Bounds)) {
                ballVelocity.x = 8.0f; // Push ball to the right
                ballVelocity.y = jumpStrength / 2; // Add slight upward force
            }

            if (ballBounds.intersects(player2Bounds)) {
                ballVelocity.x = -8.0f; // Push ball to the left
                ballVelocity.y = jumpStrength / 2; // Add slight upward force
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


            // ---------------- Swing Mechanics ----------------
            // Swing Player 1 Foot (B Key)
            static float player1SwingCharge = 0.0f; // Charge for Player 1's foot swing
            static float player1SwingSpeed = 0.0f;  // Speed for the fling-back motion

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                player1Swinging = true;
                player1SwingCharge += 2.0f; // Increment the charge gradually
                player1SwingCharge = std::min(player1SwingCharge, 70.0f); // Cap the charge at a maximum angle
                player1FootAngle = player1SwingCharge; // Swing the foot backward behind the player
            }
            else if (player1Swinging) {
                player1SwingSpeed += 2.0f; // Gradually increase the forward fling speed
                player1FootAngle += player1SwingSpeed * 0.1f + 30.0f; // Swing the foot forward slower
                if (player1FootAngle >= 70.0f) { // Reset once the forward motion is complete
                    player1SwingSpeed = 0.0f; // Reset speed
                    player1Swinging = false; // Stop swinging
                    player1FootAngle = -45.0f; // Reset angle
                    if (player1Foot.getGlobalBounds().intersects(ball.getGlobalBounds())) {
                        ballVelocity.x += player1SwingCharge * 2.0f; // Apply horizontal force to the ball
                        ballVelocity.y -= player1SwingCharge * 1.0f; // Apply vertical force to the ball
                    }
                    player1SwingCharge = 0.0f; // Reset charge after the kick
                }
            }

            // Swing Player 2 Foot (P Key)
            static float player2SwingCharge = 0.0f; // Charge for Player 2's foot swing
            static float player2SwingSpeed = 0.0f;  // Speed for the fling-back motion

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                player2Swinging = true;
                player2SwingCharge += 1.0f; // Increment the charge gradually
                player2SwingCharge = std::min(player2SwingCharge, -70.0f); // Cap the charge at a maximum angle
                player2FootAngle = player2SwingCharge; // Swing the foot backward behind the player
            }
            else if (player2Swinging) {
                player2SwingSpeed += 2.0f; // Gradually increase the forward fling speed
                player2FootAngle -= player2SwingSpeed * 0.1f; // Swing the foot forward slower
                if (player2FootAngle <= 0.0f) { // Reset once the forward motion is complete
                    player2SwingSpeed = 0.0f; // Reset speed
                    player2Swinging = false; // Stop swinging
                    player2FootAngle = 0.0f; // Reset angle
                    if (player2Foot.getGlobalBounds().intersects(ball.getGlobalBounds())) {
                        ballVelocity.x -= player2SwingCharge * 2.0f; // Apply horizontal force to the ball
                        ballVelocity.y += player2SwingCharge * 1.0f; // Apply vertical force to the ball
                    }
                    player2SwingCharge = 0.0f; // Reset charge after the kick
                }
            }


            // Update Player 1's foot position and rotation
            player1Foot.setOrigin(0, player1Foot.getSize().y / 2.0f); // Set origin to the left-middle of the foot
            player1Foot.setRotation(player1FootAngle); // Apply the swing angle
            player1Foot.setPosition(player1.getPosition().x + player1.getSize().x / 2.0f, player1.getPosition().y + player1.getSize().y); // Attach foot to bottom center of Player 1

            // Update Player 2's foot position and rotation
            player2Foot.setOrigin(player2Foot.getSize().x, player2Foot.getSize().y / 2.0f); // Set origin to the right-middle of the foot
            player2Foot.setRotation(player2FootAngle); // Apply the swing angle
            player2Foot.setPosition(player2.getPosition().x + player2.getSize().x / 2.0f, player2.getPosition().y + player2.getSize().y); // Attach foot to bottom center of Player 2




            // ---------------- Goal Check ----------------
            // Check if the ball enters the left goal
            if (ball.getPosition().x <= leftGoal.getPosition().x + leftGoal.getSize().x &&
                ball.getPosition().y + ball.getRadius() * 2 >= leftGoal.getPosition().y &&
                ball.getPosition().y <= leftGoal.getPosition().y + leftGoal.getSize().y) {
                scorePlayer2++; // Player 2 scores
                isResetting = true;
                resetClock.restart();
            }

            // Check if the ball enters the right goal
            if (ball.getPosition().x + ball.getRadius() * 2 >= rightGoal.getPosition().x &&
                ball.getPosition().y + ball.getRadius() * 2 >= rightGoal.getPosition().y &&
                ball.getPosition().y <= rightGoal.getPosition().y + rightGoal.getSize().y) {
                scorePlayer1++; // Player 1 scores
                isResetting = true;
                resetClock.restart();
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
        window.draw(field);
        window.draw(leftGoal);
        window.draw(rightGoal);
        window.draw(player1);
        window.draw(player2);
        window.draw(player1Foot);
        window.draw(player2Foot);
        window.draw(ball);
        window.display();
    }

    return 0;
}