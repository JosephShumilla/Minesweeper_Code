#include <vector>
#include "Tile.h"
#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
class Board {
public:
    int width_;
    int height_;
    int numMines_;
    int numTilesShown;
    std::vector<std::vector<Tile>> board_;
    bool gameOver_;
    bool gameWon_;
    sf::Sprite happyFace_Sprite;
    sf::Sprite debug_Sprite;
    sf::Sprite pause_Sprite;
    sf::Sprite leaderboard_Sprite;

    int getWidth(){
        return width_;
    }
    int getHeight(){
        return height_;
    }
    int getNumMines(){
        return numMines_;
    }
    void initializeBoard(){
        board_.resize(height_);
        for(int i = 0; i < height_; i++){
            board_[i].resize(width_);
            for (int j = 0; j < width_; j++){
                board_[i][j] = Tile();
            }
        }
    }
    void printBoard(sf::RenderWindow& x){
        for(int i = 0; i < height_; i++){
            for(int j = 0; j <width_; j++){
                board_[i][j].TileSprite.setPosition(j*32, i*32);
                x.draw(board_[i][j].TileSprite);
            }
        }
        sf::Texture happyFace_texture;
        sf::Texture debug_texture;
        sf::Texture pause_texture;
        sf::Texture leaderboard_texture;
        sf::Texture tile_hidden_texture;
        sf::Texture tile_revealed_texture;
        tile_hidden_texture.loadFromFile("files/images/tile_hidden.png");
        tile_revealed_texture.loadFromFile("files/images/tile_revealed.png");
        happyFace_texture.loadFromFile("files/images/face_happy.png");
        debug_texture.loadFromFile("files/images/debug.png");
        pause_texture.loadFromFile("files/images/pause.png");
        leaderboard_texture.loadFromFile("files/images/leaderboard.png");
        happyFace_Sprite.setTexture(happyFace_texture);
        happyFace_Sprite.setPosition((((width_) / 2.0) * 32) - 32, 32 *((height_)+0.5f));
        debug_Sprite.setTexture(debug_texture);
        debug_Sprite.setPosition(((width_) * 32) - 304, 32 * ((height_)+0.5f));
        pause_Sprite.setTexture(pause_texture);
        pause_Sprite.setPosition(((width_) * 32) - 240, 32 * ((height_)+0.5f));
        leaderboard_Sprite.setTexture(leaderboard_texture);
        leaderboard_Sprite.setPosition(((width_) * 32) - 176, 32 * ((height_)+0.5f));
        x.draw(happyFace_Sprite);
        x.draw(debug_Sprite);
        x.draw(pause_Sprite);
        x.draw(leaderboard_Sprite);

    }
    void placeMines(){
        int numPlacedMines = 0;
        int numMines = numMines_;
        cout << numMines << endl;
        while (numPlacedMines < numMines) {
            int randomRow = rand() % height_;
            int randomCol = rand() % width_;
            if (board_[randomRow][randomCol].hasMine()) {
                continue;
            }
            else{
                board_[randomRow][randomCol].setMine();
                numPlacedMines++;
                cout << numPlacedMines << endl;
            }
        }
    }
    void detectMinesAround(){
        for (int i = 0; i < height_; i++) {
            for (int j = 0; j < width_; j++) {
                int adjacentMines = 0;
                for (int x = -1; x <= 1; x++) {
                    for (int y = -1; y <= 1; y++) {
                        int nx = i + x;
                        int ny = j + y;
                        if (nx >= 0 && nx < height_ && ny >= 0 && ny < width_) {
                            if (board_[nx][ny].hasMine()) {
                                adjacentMines++;
                            }
                        }
                    }
                }
                board_[i][j].setMinesAround(adjacentMines);
            }
        }
    }
    void readFile(){
        ifstream numFile("files/board_config.cfg");
        if(!numFile.is_open()){
            cout << "Error: file could not be opened" << endl;
        }
        numFile >> width_;
        numFile >> height_;
        numFile >> numMines_;
    }
    Board(){
        width_ = 0;
        height_ = 0;
        numMines_ = 0;
    }

    Board(int width, int height, int numMines){
        width_ = width;
        height_ = height;
        numMines_ = numMines;
    }
    bool isGameOver(){
        return gameOver_;
    }
    bool isGameWon(){
        return gameWon_;
    }
    void revealTile(int x, int y){
        board_[x][y].TileShown();
    }
    void toggleFlag(int x, int y){
        board_[x][y].setFlag();
    }
    Tile getTile(int x, int y){
        return board_[x][y];
    }
    Vector2f getDebugPos(){
        return debug_Sprite.getPosition();
    }
    Vector2f getDebugSize(){
        return sf::Vector2f(debug_Sprite.getTexture()->getSize());
    }
    Board showMines(){
        for(int i = 0; i < height_; i++){
            for(int j = 0; j< width_; j++){
                if(board_[i][j].hasMine() && board_[i][j].isRevealed()) {
                    board_[i][j].setLayer("mine");
                }
            }
        }
    }
};
