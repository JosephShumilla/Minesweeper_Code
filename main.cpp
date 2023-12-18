#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include "LeaderBoard.h"

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

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
        font.loadFromFile("images/font.ttf");
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
    Board pauseBoard = Board("images/play.png");
    pauseBoard.readFile();
    pauseBoard.initializeBoardPaused();
    sf::RenderWindow game(sf::VideoMode(board.getWidth() * 32, (board.getHeight() * 32) + 100), "Minesweeper",
                          sf::Style::Close);
    int flagsToBePlaced = board.numMines_;

    LeaderBoard leader(int(board.getWidth()) * 32, int(board.getHeight()) * 32);

    while (game.isOpen()) {
        game.clear(sf::Color::White);
        board.setPausePlayTexture();
        board.setGameOverTexture();
        board.setGameWonTexture();
        board.printBoard(game);
        if(board.checkWinner(board.numMines_)){
            board.gameWon_ = true;
        }
        if(board.gameOver_){

            board.showMines(game);
        }
        sf::Event event;
        vector<vector<Tile>> revealedTiles;
        for(int i = 0; i<board.height_; i++){
            for(int j = 0; j<board.width_; j++){
                if(!board.board_[i][j].has_Mine) {
                    board.board_[i][j].minesAround = board.detectMinesAround(i, j);
                }
                if(board.board_[i][j].isRevealed()){
                    board.revealTilesAround(i, j);
                }
            }
        }
        while (game.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                game.close();
                break;
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(game);
                    sf::Vector2f dimension_pos = menu.mapPixelToCoords(mousePosition);
                    int x = mousePosition.x / 32;
                    int y = mousePosition.y / 32;
                    if(board.happyFace_Sprite.getGlobalBounds().contains(dimension_pos) && !board.isPaused){
                        board.resetGame();
                        continue;
                    }
                    if(board.pause_Sprite.getGlobalBounds().contains(dimension_pos)){
                        board.isPaused = !board.isPaused;
                        if (board.isPaused) {
                            board.savedtime = board.clock.getElapsedTime().asSeconds();
                            board.pause();
                        }
                        if (!board.isPaused) {
                            board.clock.restart();
                            board.unpause();
                        }
                    }
                    if(x < board.width_ && y < board.height_ && !board.board_[y][x].isRevealed() && !board.board_[y][x].hasFlag() && !board.isPaused) {
                        board.board_[y][x].setTexture("tile_revealed");
                        board.board_[y][x].TileShown();
                    }
                    if(board.debug_Sprite.getGlobalBounds().contains(dimension_pos) && !board.isPaused){
                        board.debugMode_ = !board.debugMode_;
                        board.showMines(game);
                    }
                    if(board.leaderboard_Sprite.getGlobalBounds().contains(dimension_pos)){
                        board.pause();
                        board.printBoard(game);
                        game.display();
                        leader.LeaderBoardWindow();
                        board.unpause();
                    }

                }
                if(event.mouseButton.button == sf::Mouse::Right){
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(game);
                    int x = mousePosition.x / 32;
                    int y = mousePosition.y / 32;
                    board.board_[y][x].setFlag();
                    if(x < board.width_ && y < board.height_ && !board.board_[y][x].isRevealed()) {
                        board.board_[y][x].setFlagSprite("flag");
                        flagsToBePlaced = board.countFlags();
                    }
                }

            }
        }
        game.display();
    }
    return 0;
}