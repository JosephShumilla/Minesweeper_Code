#include <string>
#include <vector>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

using namespace std;
class LeaderBoard {
    int width;
    int height;
    int rank;
    string time;
    string name;
    vector<string> leaderboardscores;

public:
    LeaderBoard(int w, int h) {
        ifstream leaderBoard("leaderboard.txt");
        string line;
        width = w;
        height = h;
        for (int i = 0; i < 5; i++) {
            getline(leaderBoard, line);
            leaderboardscores.push_back(line);

            sort(leaderboardscores.begin(), leaderboardscores.end());
        }
    }
    void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

    void LeaderBoardWindow() {
        sf::RenderWindow window(sf::VideoMode(width / 2, height / 2 + 50), "My window");
        string scores;
        for (int i = 0; i < leaderboardscores.size(); i++) {
            int comma = leaderboardscores[i].find(',');
            time = leaderboardscores[i].substr(0, 5);
            name = leaderboardscores[i].substr(6);
            int count = i + 1;
            scores += to_string(count) + ".\t" + time + "\t" + name + "\n\n";
        }
        sf::Font font;
        font.loadFromFile("files/font.ttf");
        sf::Text text;
        sf::Text title;
        title.setFont(font);
        title.setString("LEADERBOARD");
        title.setStyle(sf::Text::Bold);
        title.setStyle(sf::Text::Underlined);
        title.setPosition(width/2/2.0f, (height/2.0f) - 120);
        title.setCharacterSize(20);
        title.setOrigin(title.getLocalBounds().width/2, title.getLocalBounds().height/2 + 120);
        text.setFont(font);
        text.setString(scores);
        text.setCharacterSize(18);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::White);
        //setText(text, width/2.0f, height/2.0f + 20);
        text.setPosition(width/2/2, (height/2+50) / 2);
        text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear(sf::Color::Blue);
            window.draw(text);
            window.draw(title);
            window.display();
        }

    }
};