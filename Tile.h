
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <iostream>
class Tile {
public:
    bool has_Mine;
    bool has_flag;
    bool flagMode_;
    int minesAround;
    bool is_Shown;
    sf::Sprite TileSprite;
    sf::Sprite LayerSprite;
    sf::Sprite FlagSprite;
    Tile() {
        has_Mine = false;
        has_flag = false;
        minesAround = 0;
        is_Shown = false;
        flagMode_ = false;
        TileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
    }
    Tile(string x) {
        has_Mine = false;
        has_flag = false;
        minesAround = 0;
        is_Shown = false;
        flagMode_ = false;
        TileSprite.setTexture(TextureManager::GetTexture(x));
    }
    void setMine() {
        has_Mine = true;
    }
    bool hasMine() const{
        return has_Mine;
    }
    void setMinesAround(int numMines){
        minesAround = numMines;
    }
    int getMinesAround() const{
        return minesAround;
    }
    bool hasFlag() const {
        return has_flag;
    }
    void setFlag() {
        has_flag = !has_flag;
    }

    void set_none_Flag() {
        has_flag = false;
    }
    void TileShown(){
        is_Shown = true;
    }
    void setTileShown(bool x){
        is_Shown = x;
    }
    bool isRevealed() const {
        return is_Shown;
    }
    void setTexture(string text){
        TileSprite.setTexture(TextureManager::GetTexture(text));
    }
    void setLayer(string text){
        LayerSprite.setTexture(TextureManager::GetTexture(text));
    }
    void setFlagSprite(string text){
        FlagSprite.setTexture(TextureManager::GetTexture(text));
    }
    void tempReveal(sf::RenderWindow &x){
        sf::Sprite reveal_sprite = TileSprite;
        sf::Texture reveal_texture;
        reveal_texture.loadFromFile("images/tile_revealed.png");
        reveal_sprite.setTexture(reveal_texture);
        x.draw(reveal_sprite);
    }
};