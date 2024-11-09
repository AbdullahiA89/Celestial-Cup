// ---------------- Includes ----------------
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

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
    if (!font.loadFromFile("bin/debug/res/Pixelify_Sans/PixelifySans-VariableFont_wght.ttf")) {
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

        // Render based on game state
        window.clear(sf::Color::Black);

        if (currentGameState == MainMenu) {
            // Render Main Menu
            window.draw(titleText);
            window.draw(startText);
            window.draw(quitText);
        }
        else if (currentGameState == Playing) {
            // Render Gameplay Scene
            window.draw(field);
            window.draw(player1);
            window.draw(player2);
        }

        window.display();
    }

    return 0;
}
