#include <vector>
#include "Tile.h"
#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
using namespace std;
class Board {
public:
    bool firsttimearound = true;
    int width_;
    int height_;
    int numMines_;
    int numMinesFlagged;
    int numberDisplayed;
    float savedtime = 0;
    bool isPaused;
    std::vector<std::vector<Tile>> board_;
    bool gameOver_;
    bool gameWon_;
    bool debugMode_;
    int clock1_digit;
    int clock2_digit;
    int clock3_digit;
    int clock4_digit;
    int totalseconds;
    sf::Clock clock;
    vector<vector<Tile>> revealedTiles;
    sf::Sprite happyFace_Sprite;
    sf::Sprite debug_Sprite;
    sf::Sprite pause_Sprite;
    sf::Sprite play_Sprite;
    sf::Sprite leaderboard_Sprite;
    sf::Sprite clock_1Sprite;
    sf::Sprite clock_2Sprite;
    sf::Sprite clock_3Sprite;
    sf::Sprite clock_4Sprite;
    sf::Sprite bomb_dig1;
    sf::Sprite bomb_dig2;
    sf::Sprite bomb_dig3;
    sf::Sprite bomb_dig4;
    sf::Texture happyFace_texture;
    sf::Texture debug_texture;
    sf::Texture pause_texture;
    sf::Texture play_texture;
    sf::Texture leaderboard_texture;
    sf::Texture tile_hidden_texture;
    sf::Texture tile_revealed_texture;
    sf::Texture clock_Texture;
    int getWidth() {
        return width_;
    }

    int getHeight() {
        return height_;
    }

    int getNumMines() {
        return numMines_;
    }

    void initializeBoard() {
        board_.resize(height_);
        for (int i = 0; i < height_; i++) {
            board_[i].resize(width_);
            for (int j = 0; j < width_; j++) {
                board_[i][j] = Tile();
            }
        }
        clock.restart();
    }

    void initializeBoardPaused() {
        board_.resize(height_);
        for (int i = 0; i < height_; i++) {
            board_[i].resize(width_);
            for (int j = 0; j < width_; j++) {
                board_[i][j] = Tile("tile_revealed");
            }
        }
    }

    void printBoard(sf::RenderWindow &x) {
        if (isPaused){
            for (int i = 0; i < height_; i++) {
                for (int j = 0; j < width_; j++) {
                    board_[i][j].tempReveal(x);
                }
            }
        }
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                if(isPaused)
                    continue;
                if (board_[i][j].minesAround == 1 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_1");
                } else if (board_[i][j].minesAround == 2 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_2");
                } else if (board_[i][j].minesAround == 3 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_3");
                } else if (board_[i][j].minesAround == 4 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_4");
                } else if (board_[i][j].minesAround == 5 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_5");
                } else if (board_[i][j].minesAround == 6 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_6");
                } else if (board_[i][j].minesAround == 7 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_7");
                } else if (board_[i][j].minesAround == 8 and board_[i][j].is_Shown) {
                    board_[i][j].setLayer("number_8");
                }
                if(board_[i][j].has_Mine){
                    board_[i][j].setLayer("mine");
                }
                if(board_[i][j].has_Mine && gameOver_){
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                }
                if (!(board_[i][j].isRevealed()))
                    board_[i][j].TileSprite.setPosition(j * 32, i * 32);
                x.draw(board_[i][j].TileSprite);
                if (board_[i][j].hasMine() && (debugMode_ || gameOver_)) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                }
                if(board_[i][j].is_Shown && !board_[i][j].has_Mine){
                    board_[i][j].setTexture("tile_revealed");
                    x.draw(board_[i][j].TileSprite);
                }
                if (board_[i][j].hasFlag() && !(board_[i][j].is_Shown) && !board_[i][j].flagMode_) {
                    board_[i][j].FlagSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].FlagSprite);
                }
                if (board_[i][j].has_Mine && board_[i][j].isRevealed() && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                }
                if (board_[i][j].minesAround == 1 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                } else if (board_[i][j].minesAround == 2 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                } else if (board_[i][j].minesAround == 3 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                } else if (board_[i][j].minesAround == 4 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                } else if (board_[i][j].minesAround == 5 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                } else if (board_[i][j].minesAround == 6 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                } else if (board_[i][j].minesAround == 7 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                } else if (board_[i][j].minesAround == 8 && board_[i][j].is_Shown) {
                    board_[i][j].LayerSprite.setPosition(j * 32, i * 32);
                    x.draw(board_[i][j].LayerSprite);
                }
                if(board_[i][j].is_Shown and board_[i][j].has_Mine){
                    gameOver_ = true;
                    if (firsttimearound) {
                        savedtime = clock.restart().asSeconds();
                        firsttimearound = false;
                    }
                }
            }
        }
        happyFace_Sprite.setTexture(happyFace_texture);
        happyFace_Sprite.setPosition((((width_) / 2.0) * 32) - 32, 32 * ((height_) + 0.5f));
        debug_Sprite.setTexture(debug_texture);
        debug_Sprite.setPosition(((width_) * 32) - 304, 32 * ((height_) + 0.5f));
        pause_Sprite.setTexture(pause_texture);
        pause_Sprite.setPosition(((width_) * 32) - 240, 32 * ((height_) + 0.5f));
        leaderboard_Sprite.setTexture(leaderboard_texture);
        leaderboard_Sprite.setPosition(((width_) * 32) - 176, 32 * ((height_) + 0.5f));
        clock_1Sprite.setTexture(clock_Texture);
        clock_2Sprite.setTexture(clock_Texture);
        clock_3Sprite.setTexture(clock_Texture);
        clock_4Sprite.setTexture(clock_Texture);
        bomb_dig1.setTexture(clock_Texture);
        bomb_dig2.setTexture(clock_Texture);
        bomb_dig3.setTexture(clock_Texture);
        bomb_dig4.setTexture(clock_Texture);

        bomb_dig1.setPosition(33, 32 * ((height_)+0.5f)+16);
        bomb_dig2.setPosition(33+21, 32 * ((height_)+0.5f)+16);
        bomb_dig3.setPosition(33 + 42, 32 * ((height_)+0.5f)+16);
        bomb_dig4.setPosition(12, 32 *((height_)+0.5f)+16);

        int onesplace;
        int tensplace;
        int hundredsplace;
        bool isnegative = false;
        int temp = countFlags();
        if (temp < 0) {
            isnegative = true;
            temp = temp * -1;
        }
        onesplace = temp % 10;
        tensplace = temp / 10 % 10;
        hundredsplace = temp /10 /10 % 10;


        bomb_dig4.setTextureRect(sf::IntRect(210, 0, 21, 32));
        bomb_dig1.setTextureRect(sf::IntRect(hundredsplace * 21, 0, 21, 32));
        bomb_dig2.setTextureRect(sf::IntRect(tensplace * 21, 0, 21, 32));
        bomb_dig3.setTextureRect(sf::IntRect(onesplace * 21, 0, 21, 32));

        clock_1Sprite.setTextureRect(sf::IntRect(clock1_digit * 21, 0, 21, 32));
        clock_2Sprite.setTextureRect(sf::IntRect(clock2_digit * 21, 0, 21, 32));
        clock_3Sprite.setTextureRect(sf::IntRect(clock3_digit * 21, 0, 21, 32));
        clock_4Sprite.setTextureRect(sf::IntRect(clock4_digit * 21, 0, 21, 32));
        x.draw(happyFace_Sprite);
        x.draw(debug_Sprite);
        x.draw(pause_Sprite);
        x.draw(leaderboard_Sprite);
        x.draw(bomb_dig1);
        x.draw(bomb_dig2);
        x.draw(bomb_dig3);
        if (isnegative)
            x.draw(bomb_dig4);

        totalseconds = savedtime;
        if (!isPaused and !gameOver_)
            totalseconds += clock.getElapsedTime().asSeconds();
        int minutesdigit2 = totalseconds/60;
        int minutesdigit1 = (totalseconds/60)/10 % 10;
        int secondsdigit1 = (totalseconds/10) % 10;
        int secondsdigit2 = totalseconds % 10;
        if(secondsdigit1 > 5){
            secondsdigit1 = 0;
        }
        clock_1Sprite.setTextureRect(sf::IntRect( minutesdigit1 *21, 0, 21, 32));
        clock_2Sprite.setTextureRect(sf::IntRect( minutesdigit2 *21, 0, 21, 32));
        clock_3Sprite.setTextureRect(sf::IntRect( secondsdigit1 *21, 0, 21, 32));
        clock_4Sprite.setTextureRect(sf::IntRect( secondsdigit2 *21, 0, 21, 32));
        clock_1Sprite.setPosition(((width_)*32)-97, 32*((height_)+0.5f)+16);
        clock_2Sprite.setPosition(((width_)*32)-97+21, 32*((height_)+0.5f)+16);
        clock_3Sprite.setPosition(((width_)*32)-54, 32*((height_)+0.5f)+16);
        clock_4Sprite.setPosition(((width_)*32)-33, 32*((height_)+0.5f)+16);
        x.draw(clock_1Sprite);
        x.draw(clock_2Sprite);
        x.draw(clock_3Sprite);
        x.draw(clock_4Sprite);
    }
    void printBoardPaused(sf::RenderWindow &x) {
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                board_[i][j].TileSprite.setPosition(j * 32, i * 32);
                x.draw(board_[i][j].TileSprite);
            }
        }
        happyFace_Sprite.setTexture(happyFace_texture);
        happyFace_Sprite.setPosition((((width_) / 2.0) * 32) - 32, 32 * ((height_) + 0.5f));
        debug_Sprite.setTexture(debug_texture);
        debug_Sprite.setPosition(((width_) * 32) - 304, 32 * ((height_) + 0.5f));
        play_Sprite.setTexture(play_texture);
        play_Sprite.setPosition(((width_) * 32) - 240, 32 * ((height_) + 0.5f));
        leaderboard_Sprite.setTexture(leaderboard_texture);
        leaderboard_Sprite.setPosition(((width_) * 32) - 176, 32 * ((height_) + 0.5f));
        x.draw(happyFace_Sprite);
        x.draw(debug_Sprite);
        x.draw(play_Sprite);
        x.draw(leaderboard_Sprite);

    }

    void placeMines() {
        int numPlacedMines = 0;
        int numMines = numMines_;
        while (numPlacedMines < numMines) {
            int randomRow = rand() % height_;
            int randomCol = rand() % width_;
            if (board_[randomRow][randomCol].hasMine()) {
                continue;
            } else {
                board_[randomRow][randomCol].setMine();
                numPlacedMines++;
            }
        }
    }

    int detectMinesAround(int x, int y) {
        int mineCnt = 0;
        // x+1, y
        if (x + 1 < height_ and board_[x + 1][y].has_Mine)
            mineCnt++;
        // x, y+1
        if (y + 1 < width_ and board_[x][y + 1].has_Mine) {
            mineCnt++;
        }
        // x, y-1
        if (y - 1 >= 0 and board_[x][y - 1].has_Mine) {
            mineCnt++;
        }
        // x-1, y
        if (x - 1 >= 0 and board_[x - 1][y].has_Mine) {
            mineCnt++;
        }
        // x+1, y+1
        if ((x + 1 < height_ && y + 1 < width_) and board_[x + 1][y + 1].has_Mine) {
            mineCnt++;
        }
        // x+1, y-1
        if ((x + 1 < height_ && y - 1 >= 0) and board_[x + 1][y - 1].has_Mine) {
            mineCnt++;
        }
        // x-1, y+1
        if ((x - 1 >= 0 && y + 1 < width_) and board_[x - 1][y + 1].has_Mine) {
            mineCnt++;
        }
        // x-1, y-1
        if ((x - 1 >= 0 && y - 1 >= 0) and board_[x - 1][y - 1].has_Mine) {
            mineCnt++;
        }
        return mineCnt;
    }

    void readFile() {
        ifstream numFile("config.cfg");
        if (!numFile.is_open()) {
            cout << "Error: file could not be opened" << endl;
        }
        numFile >> width_;
        numFile >> height_;
        numFile >> numMines_;
    }

    void fillRevealedTiles() {
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                if (board_[i][j].is_Shown) {
                    revealedTiles.push_back(board_[i]);
                }
            }
        }
    }

    Board() {
        width_ = 0;
        height_ = 0;
        numMines_ = 0;
        isPaused = false;
        debugMode_ = false;
        gameWon_ = false;
        gameOver_ = false;
        clock1_digit = 0;
        clock2_digit = 0;
        clock3_digit = 0;
        clock4_digit = 0;
        tile_hidden_texture.loadFromFile("images/tile_hidden.png");
        tile_revealed_texture.loadFromFile("images/tile_revealed.png");
        happyFace_texture.loadFromFile("images/face_happy.png");
        debug_texture.loadFromFile("images/debug.png");
        pause_texture.loadFromFile("images/pause.png");
        leaderboard_texture.loadFromFile("images/leaderboard.png");
        clock_Texture.loadFromFile("images/digits.png");
    }

    Board(string x) {
        width_ = 0;
        height_ = 0;
        numMines_ = 0;
        debugMode_ = false;
        gameWon_ = false;
        gameOver_ = false;
        tile_hidden_texture.loadFromFile("images/tile_hidden.png");
        tile_revealed_texture.loadFromFile("images/tile_revealed.png");
        happyFace_texture.loadFromFile("images/face_happy.png");
        debug_texture.loadFromFile("images/debug.png");
        leaderboard_texture.loadFromFile("images/leaderboard.png");
    }

    Board(int width, int height, int numMines) {
        width_ = width;
        height_ = height;
        numMines_ = numMines;
    }

    bool isGameOver() {
        return gameOver_;
    }

    bool isGameWon() {
        return gameWon_;
    }

    void revealTile(int x, int y) {
        board_[x][y].TileShown();
    }

    void toggleFlag(int x, int y) {
        board_[x][y].setFlag();
    }

    Tile getTile(int x, int y) {
        return board_[x][y];
    }

    Vector2f getDebugPos() {
        return debug_Sprite.getPosition();
    }

    Vector2f getDebugSize() {
        return sf::Vector2f(debug_Sprite.getTexture()->getSize());
    }

    void showMines(sf::RenderWindow &x) {
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                if (board_[i][j].hasMine()) {
                    board_[i][j].setLayer("mine");
                }
            }
        }
    }

    void resetGame() {
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                board_[i][j].has_Mine = false;
                board_[i][j].has_flag = false;
                board_[i][j].is_Shown = false;
                board_[i][j].minesAround = 0;
                clock.restart();
                savedtime = 0;
            }
        }
        firsttimearound = true;
        gameOver_ = false;
        gameWon_ = false;
        this->initializeBoard();
        this->placeMines();
    }
    void setGameOverTexture(){
        if(gameOver_){
            happyFace_texture.loadFromFile("images/face_lose.png");
        }else{
            happyFace_texture.loadFromFile("images/face_happy.png");
        }
    }
    void setGameWonTexture(){
        if(gameWon_){
            happyFace_texture.loadFromFile("images/face_win.png");
        }
    }
    void setPausePlayTexture(){
        if(isPaused){
            pause_texture.loadFromFile("images/play.png");
        }else{
            pause_texture.loadFromFile("images/pause.png");
        }
    }
    void revealTilesAround(int x , int y){
        if(x>height_ || y>width_ || x < 0 || y < 0){
            return;
        }
        if(detectMinesAround(x, y) > 0 || board_[x][y].has_flag || board_[x][y].has_Mine){
            return;
        }
        for(int i = x - 1; i <= x + 1; i++){
            for(int j = y - 1; j <= y + 1; j++){
                if(i >= 0 and j>= 0 and i < height_ and j <width_) {
                    board_[i][j].is_Shown = true;
                    if (board_[i][j].is_Shown == false) {
                        revealTilesAround(i, j);
                    }
                }
            }
        }
    }
    void pause() {
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                board_[i][j].setTexture("tile_revealed");
            }
        }
    }
    void unpause() {
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                if (!board_[i][j].is_Shown)
                    board_[i][j].setTexture("tile_hidden");
            }
        }
    }
    int countFlags(){
        int flags = 0;
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                if(board_[i][j].has_flag){
                    flags++;
                }
            }
        }
        return numMines_ - flags;
    }
    bool checkWinner(int mines){
        int revealedtiles = 0;
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                if(!board_[i][j].is_Shown){
                    revealedtiles++;
                }
            }
        }
        if(mines == revealedtiles){
            return true;
        }else{
            return false;
        }
    }
};