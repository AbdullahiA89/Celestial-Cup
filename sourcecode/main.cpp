#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

int main() {
    // Create a 1080p window with a frame rate limit of 60 FPS
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Celestial Cup", sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);

    // Field (Rectangle) - Covers the entire window as the football field
    sf::RectangleShape field(sf::Vector2f(1920.0f, 1080.0f));
    field.setFillColor(sf::Color(50, 150, 50)); // Green color for grass

    // Player 1 (Blue Rectangle) - Positioned on the left side
    sf::RectangleShape player1(sf::Vector2f(50.0f, 100.0f)); // Width = 50, Height = 100
    player1.setFillColor(sf::Color::Blue);
    player1.setPosition(200.0f, 880.0f); // Near the left side of the field

    // Player 2 (Red Rectangle) - Positioned on the right side
    sf::RectangleShape player2(sf::Vector2f(50.0f, 100.0f)); // Width = 50, Height = 100
    player2.setFillColor(sf::Color::Red);
    player2.setPosition(1670.0f, 880.0f); // Near the right side of the field

    // Ball (White Circle) - Centered on the field
    sf::CircleShape ball(25.0f); // Radius = 25
    ball.setFillColor(sf::Color::White);
    ball.setPosition(945.0f, 880.0f); // Centered horizontally and vertically

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close the window when the user clicks "X"
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear the window with the default background color
        window.clear();

        // Draw the field, players, and ball
        window.draw(field);
        window.draw(player1);
        window.draw(player2);
        window.draw(ball);

        // Display everything on the window
        window.display();
    }

    return 0;
}