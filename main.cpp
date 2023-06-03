#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
int main() {
    sf::RenderWindow menu(sf::VideoMode(800, 600), "SFML window", sf::Style::Close);
    sf::Font font;
    std::string input;
    int cursorPosition = 0;
    sf::RectangleShape cursor(sf::Vector2f(2.0f, 24.0f));
    cursor.setFillColor(sf::Color::White);
    bool gameWindowIsOpen = false;
    while (menu.isOpen() and !gameWindowIsOpen) {
        menu.clear(sf::Color::Blue);
        sf::Text name("", font, 24);
        name.setString(input);
        sf::Text welcome;
        sf::Text enter;
        sf::FloatRect middle;
        middle.width = 400;
        font.loadFromFile("files/font.ttf");
        welcome.setFont(font);
        enter.setFont(font);
        welcome.setString("WELCOME TO MINESWEEPER!");
        enter.setString("Enter your name: ");
        name.setString(input + "|");
        welcome.setStyle(sf::Text::Bold);
        enter.setStyle(sf::Text::Bold);
        name.setStyle(sf::Text::Bold);
        welcome.setStyle(sf::Text::Underlined);
        welcome.setCharacterSize(24);
        enter.setCharacterSize(20);
        name.setCharacterSize(18);
        name.setFillColor(sf::Color::Yellow);
        welcome.setOrigin(welcome.getLocalBounds().width / 2.0f, welcome.getLocalBounds().height / 2.0f);
        welcome.setPosition(menu.getSize().x / 2.0f, menu.getSize().y / 2.0f - (150));
        enter.setOrigin(enter.getLocalBounds().width / 2.0f, enter.getLocalBounds().height / 2.0f);
        enter.setPosition(menu.getSize().x / 2.0f, menu.getSize().y / 2.0f - 75);
        name.setOrigin(name.getLocalBounds().width, name.getLocalBounds().height);
        name.setPosition(menu.getSize().x / 2.0f, menu.getSize().y / 2.0f - 45);
        welcome.setFillColor(sf::Color::White);
        enter.setFillColor(sf::Color::White);

        menu.draw(welcome);
        menu.draw(enter);
        menu.setVerticalSyncEnabled(true);
        sf::Event event;
        while (menu.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                menu.close();
                return 0;
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && input.size() > 0) {
                    input.pop_back();
                    cursorPosition--;
                } else if (isalpha(event.text.unicode) && input.size() < 10) {
                    input.insert(cursorPosition, 1, static_cast<char>((event.text.unicode)));
                    cursorPosition++;
                }
                if (isupper(input[0]) != true) {
                    input[0] = toupper(input[0]);
                }
                for (int i = 1; i < input.size(); i++) {
                    if (isupper(input[i])) {
                        input[i] = tolower(input[i]);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) and input.size() > 0) {
                    menu.close();
                    gameWindowIsOpen = true;
                    break;
                }
            }
        }
        cursor.setPosition(name.findCharacterPos(cursorPosition).x, 0.0f);
        menu.draw(name);
        menu.display();
    }
    Board board = Board();
    board.readFile();
    board.initializeBoard();
    board.placeMines();
    sf::RenderWindow game(sf::VideoMode(board.getWidth() * 32, (board.getHeight() * 32) + 100), "Minesweeper",
                          sf::Style::Close);
    while (game.isOpen()) {
        game.clear(sf::Color::White);
        board.printBoard(game);
        sf::Event event;
        while (game.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                game.close();
                break;
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(game);
                    int x = mousePosition.x / 32;
                    int y = mousePosition.y / 32;
                    if(x < board.width_ && y < board.height_ && !board.board_[y][x].isRevealed()) {
                        board.board_[y][x].setTexture("tile_revealed");
                        board.board_[y][x].TileShown();
                    }
                    if(board.debug_Sprite.getGlobalBounds().contains(x*32,y*32)){
                        board.showMines();
                    }
                }
            }
        }
        game.display();
    }
    return 0;
}