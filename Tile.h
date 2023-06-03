#include <SFML/Graphics.hpp>
#include "TextureManager.h"
class Tile {
    public:
        bool has_Mine;
        bool has_flag;
        int minesAround;
        bool is_Shown;
        sf::Sprite TileSprite;
        sf::Sprite LayerSprite;
        Tile() {
            has_Mine = false;
            has_flag = false;
            minesAround = 0;
            is_Shown = false;
            TileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
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
            has_flag = true;
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
    };
