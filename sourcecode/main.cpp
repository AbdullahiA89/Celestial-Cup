#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath> // For std::sin

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Celestial Cup", sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);

    // Field (Rectangle)
    sf::RectangleShape field(sf::Vector2f(1920.0f, 1080.0f));
    field.setFillColor(sf::Color(50, 150, 50)); // Green grass

    // Player 1 (Blue Rectangle)
    sf::RectangleShape player1(sf::Vector2f(50.0f, 100.0f));
    player1.setFillColor(sf::Color::Blue);
    sf::Vector2f player1StartPos(200.0f, 880.0f);
    player1.setPosition(player1StartPos);

    // Player 2 (Red Rectangle)
    sf::RectangleShape player2(sf::Vector2f(50.0f, 100.0f));
    player2.setFillColor(sf::Color::Red);
    sf::Vector2f player2StartPos(1670.0f, 880.0f);
    player2.setPosition(player2StartPos);

    // Player 1 Foot
    sf::RectangleShape player1Foot(sf::Vector2f(50.0f, 10.0f)); // Rectangular foot
    player1Foot.setFillColor(sf::Color::Cyan);

    // Player 2 Foot
    sf::RectangleShape player2Foot(sf::Vector2f(50.0f, 10.0f)); // Rectangular foot
    player2Foot.setFillColor(sf::Color::Magenta);

    // Ball (Circle)
    sf::CircleShape ball(25.0f);
    ball.setFillColor(sf::Color::White);
    sf::Vector2f ballStartPos(945.0f, 540.0f);
    ball.setPosition(ballStartPos);

    // Ball physics variables
    sf::Vector2f ballVelocity(0.0f, 0.0f); // Ball's velocity (x, y)
    const float ballFriction = 0.70f; // Friction for slowing ball horizontally
    const float ballBounce = -0.7f; // Ball bounces with reduced velocity
    const float kickStrength = 20.0f; // Strength of the kick

    // Goals
    sf::RectangleShape leftGoal(sf::Vector2f(50.0f, 200.0f));
    leftGoal.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
    leftGoal.setPosition(0.0f, 680.0f);

    sf::RectangleShape rightGoal(sf::Vector2f(50.0f, 200.0f));
    rightGoal.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
    rightGoal.setPosition(1870.0f, 680.0f);

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

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!isResetting) {
            // Player 1 Movement (WAD)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player1.getPosition().x > 0)
                player1.move(-5.0f, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player1.getPosition().x + player1.getSize().x < 1920)
                player1.move(5.0f, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player1OnGround) {
                player1VelocityY = jumpStrength;
                player1OnGround = false;
            }

            // Player 2 Movement (Arrow Keys)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player2.getPosition().x > 0)
                player2.move(-5.0f, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player2.getPosition().x + player2.getSize().x < 1920)
                player2.move(5.0f, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player2OnGround) {
                player2VelocityY = jumpStrength;
                player2OnGround = false;
            }

            // Apply gravity
            player1VelocityY += gravity;
            player2VelocityY += gravity;

            // Update vertical positions
            player1.move(0, player1VelocityY);
            player2.move(0, player2VelocityY);

            // Check ground collision
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
            // Ball Physics
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


            // Swing Player 1 Foot (B Key)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                player1Swinging = true;
                player1FootAngle = -45.0f;
            }
            else {
                player1Swinging = false;
                player1FootAngle = 0.0f;
            }

            // Swing Player 2 Foot (P Key)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                player2Swinging = true;
                player2FootAngle = 45.0f;
            }
            else {
                player2Swinging = false;
                player2FootAngle = 0.0f;
            }

            float kickCharge = 0.0f;
            const float maxKickStrength = 30.0f;

            // Charging kick power for Player 1
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                kickCharge += 0.5f; // Increase charge over time
                if (kickCharge > maxKickStrength) {
                    kickCharge = maxKickStrength; // Cap the charge
                }
            }

            // Apply kick and reset charge
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::B) && player1Swinging) {
                ballVelocity.x = kickCharge; // Apply the charged power
                ballVelocity.y = -kickCharge / 2; // Optional upward force
                kickCharge = 0.0f; // Reset the charge after kicking
            }

            // Update foot positions and rotations
            player1Foot.setOrigin(0, 5.0f); // Pivot point at the bottom
            player1Foot.setRotation(player1FootAngle);
            player1Foot.setPosition(player1.getPosition().x + 25.0f, player1.getPosition().y + 100.0f); // Bottom center

            player2Foot.setOrigin(player2Foot.getSize().x, 5.0f); // Pivot point at the bottom-right corner
            player2Foot.setRotation(player2FootAngle);
            player2Foot.setPosition(player2.getPosition().x + 25.0f, player2.getPosition().y + 100.0f); // Bottom center facing Player 1


            // Check for goals
            if (ball.getPosition().x <= 0) { // Player 2 scores
                scorePlayer2++;
                isResetting = true;
                resetClock.restart();
            }

            if (ball.getPosition().x + ball.getRadius() * 2 >= 1920) { // Player 1 scores
                scorePlayer1++;
                isResetting = true;
                resetClock.restart();
            }
        }
        else {
            // Resetting state: Pause game for 3 seconds
            if (resetClock.getElapsedTime().asSeconds() >= 3) {
                isResetting = false;
                player1.setPosition(player1StartPos);
                player2.setPosition(player2StartPos);
                ball.setPosition(ballStartPos);
                ballVelocity = sf::Vector2f(0.0f, 0.0f); // Stop the ball
            }
        }

        // Clear the window
        window.clear();

        // Draw the field, players, feet, ball, and goals
        window.draw(field);
        window.draw(leftGoal);
        window.draw(rightGoal);
        window.draw(player1);
        window.draw(player2);
        window.draw(player1Foot);
        window.draw(player2Foot);
        window.draw(ball);

        // Display everything
        window.display();
    }

    return 0;
}