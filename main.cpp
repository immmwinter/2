#include <iostream>
#include <SFML/Graphics.hpp>
#include "test_button.h"
#include "graph.h"
#include "scene.h"

using namespace std;

class Game {
public:
    Game() :
        window(sf::VideoMode(1080, 720), "Stock Chart"),
        btn1("Next (10)", { 100, 50 }, 30, sf::Color::Green, sf::Color::Black),
        stockGraph(550.0f, 300.0f),
        countdown(15),
        currentScene(SceneState::GRAPH) // เริ่มจาก Graph Scene
    {
        srand(time(0));
        loadAssets();
        setupComponents();

        btn1.setfont(font);
        btn1.setPosition({ 800, 600 });
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::Font font;
    sf::Text titleText;
    sf::RectangleShape chartArea;
    Button btn1;
    StockGraph stockGraph;
    SceneState currentScene;

    sf::Clock countdownClock;
    int countdown;

    void loadAssets() {
        if (!bgTexture.loadFromFile("C:/Users/getbh/source/repos/last/2/element/pl.jpg")) {
            cout << "Failed to load background texture" << endl;
            system("pause");
        }

        if (!font.loadFromFile("C:/Users/getbh/source/repos/last/2/element/Rainbow Memories.otf")) {
            cout << "Failed to load font" << endl;
            system("pause");
        }
    }

    void setupComponents() {
        bgSprite.setTexture(bgTexture);
        bgSprite.setTextureRect(sf::IntRect(0, 0, 1080, 720));
        bgSprite.setPosition(0, 0);

        chartArea.setSize(sf::Vector2f(750.0f, 400.0f));
        chartArea.setPosition(150, 150);
        chartArea.setFillColor(sf::Color::White);
        chartArea.setOutlineThickness(1);
        chartArea.setOutlineColor(sf::Color::Black);

        titleText.setFont(font);
        titleText.setString("Stock Chart");
        titleText.setCharacterSize(50);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(100, 50);
        titleText.setOutlineThickness(3);
        titleText.setOutlineColor(sf::Color::White);
    }

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (btn1.isMouseOver(window)) {
                    cout << "Next round (Manual update)" << endl;
                    countdown = 15;
                    currentScene = SceneState::PLAYING; // เปลี่ยนฉากไป PLAYING
                }
            }
        }
    }

    void update() {
        if (countdownClock.getElapsedTime().asSeconds() >= 1) {
            if (countdown > 0) {
                countdown--;
            }
            else {
                cout << "Next round (Auto update)" << endl;
                currentScene = SceneState::PLAYING; // เปลี่ยนเป็น PLAYING อัตโนมัติ
                countdown = 15;
            }
            btn1.setText("Next in " + to_string(countdown) + "");
            countdownClock.restart();
        }
    }

    void render() {
        window.clear();
        window.draw(bgSprite);

        if (currentScene == SceneState::GRAPH) {
            window.draw(titleText);
            window.draw(chartArea);
            stockGraph.drawGraph(window, chartArea);
        }
        
        btn1.drawTo(window);
        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
