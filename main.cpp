#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>     
#include <cstdlib>   
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
        currentScene(SceneState::GRAPH) // เริ่มที่หน้า GRAPH
    {
        srand(time(0)); // ตั้งค่าการสุ่ม
        loadAssets();
        setupComponents();
        loadNews(); // โหลดข่าวก่อน

        if (!newsList.empty()) {
            updateNews(); // 🔥 เรียกให้ข่าวสุ่มตั้งแต่แรก
        }

        btn1.setFont(font);
        btn1.setPosition({ 800, 565 });
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
    sf::Text newsText;
    sf::RectangleShape chartArea;
    Button btn1;
    StockGraph stockGraph;
    SceneState currentScene;

    vector<string> newsList;
    vector<int> newsEffects;
    int currentNewsIndex;

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

        chartArea.setSize(sf::Vector2f(800.0f, 400.0f));
        chartArea.setPosition(135, 125);
        chartArea.setFillColor(sf::Color::White);
        chartArea.setOutlineThickness(1);
        chartArea.setOutlineColor(sf::Color::Black);

        titleText.setFont(font);
        titleText.setString("Market Trend");
        titleText.setCharacterSize(60);
        titleText.setFillColor(sf::Color(0,200,83));
        titleText.setPosition(165, 40);
        titleText.setOutlineThickness(2);
        titleText.setOutlineColor(sf::Color::Black);

        newsText.setFont(font);
        newsText.setCharacterSize(30);
        newsText.setFillColor(sf::Color::Black);
        newsText.setPosition(150, 550);
        newsText.setOutlineThickness(1);
        newsText.setOutlineColor(sf::Color::White); //nk
    }
    
    void loadNews() {
        ifstream file("C:/Users/getbh/source/repos/last/2/news/news.txt");
        if (!file) {
            cout << "Failed to open news file" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find_last_of(" ");
            if (pos != string::npos) {
                string newsContent = line.substr(0, pos);
                string effectStr = line.substr(pos + 1);

                newsContent.erase(newsContent.find_last_not_of(" ") + 1);
                effectStr.erase(0, effectStr.find_first_not_of(" "));

                try {
                    int effect = stoi(effectStr);
                    newsList.push_back(newsContent);
                    newsEffects.push_back(effect);
                }
                catch (exception& e) {
                    cout << "Error parsing effect: " << effectStr << " in line: " << line << endl;
                }
            }
        }
        file.close();

        cout << "Loaded " << newsList.size() << " news articles." << endl; // ตรวจสอบว่ามีกี่ข่าวถูกโหลดมา
        for (size_t i = 0; i < newsList.size(); i++) {
            cout << "News " << i << ": " << newsList[i] << " (" << newsEffects[i] << ")" << endl;
        }

        if (!newsList.empty()) {
            updateNewsText();
            applyNewsEffect();
        }
    }


    void updateNews() {
        if (!newsList.empty()) {
            int newIndex;
            do {
                newIndex = rand() % newsList.size(); // สุ่มข่าวใหม่
            } while (newsList.size() > 1 && newIndex == currentNewsIndex); // หลีกเลี่ยงข่าวซ้ำถ้ามีหลายอัน

            currentNewsIndex = newIndex;
            applyNewsEffect();
        }
    }



    void applyNewsEffect() {
        if (!newsEffects.empty()) {
            stockGraph.setNewsEffect(newsEffects[currentNewsIndex]);
            stockGraph.resetGraph();
            updateNewsText();
        }
    }

    void updateNewsText() {
        if (!newsList.empty()) {
            newsText.setString("Market News:\n" + newsList[currentNewsIndex] + " (" + to_string(newsEffects[currentNewsIndex]) + ")");
        }
    }


    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (btn1.isMouseOver(window)) {
                    cout << "Next round (Manual update)" << endl;
                    updateNews();  // ให้ข่าวเปลี่ยนเมื่อกดปุ่ม
                    countdown = 15;
                    currentScene = SceneState::PLAYING;
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
                updateNews();      // สุ่มข่าวใหม่
                applyNewsEffect(); // อัปเดตกราฟตามข่าวที่สุ่มได้
                currentScene = SceneState::PLAYING;
                countdown = 15;
            }
            btn1.setText("Next in " + to_string(countdown) + "s");
            countdownClock.restart();
        }

        if (currentScene == SceneState::PLAYING) {
            btn1.hide(); // ซ่อนปุ่มเมื่ออยู่ในหน้า PLAYING
        }
        else {
            btn1.show(); // แสดงปุ่มเมื่อกลับมาหน้า GRAPH
        }
    }


    void render() {
        window.clear();
        window.draw(bgSprite);

        if (currentScene == SceneState::GRAPH) {
            window.draw(titleText);
            window.draw(chartArea);
            stockGraph.drawGraph(window, chartArea);
            window.draw(newsText);
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
