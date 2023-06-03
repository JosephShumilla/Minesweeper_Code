#include "TextureManager.h"
#include <unordered_map>
unordered_map<string, Texture>  TextureManager::textures;

void TextureManager::LoadTexture(string name) {
    string path = "files/images/" + name + ".png";
    textures[name].loadFromFile(path);
}

Texture& TextureManager::GetTexture(string name) {
    if (textures.find(name) == textures.end()) {
        LoadTexture(name);
    }
    return textures[name];
}

void TextureManager::SetDigits() {
    string path = "images/digits.png";

    textures["0"].loadFromFile(path, IntRect(0, 0, 21, 32));
    textures["1"].loadFromFile(path, IntRect(21, 0, 21, 32));
    textures["2"].loadFromFile(path, IntRect(21*2, 0, 21, 32));
    textures["3"].loadFromFile(path, IntRect(21*3, 0, 21, 32));
    textures["4"].loadFromFile(path, IntRect(21*4, 0, 21, 32));
    textures["5"].loadFromFile(path, IntRect(21*5, 0, 21, 32));
    textures["6"].loadFromFile(path, IntRect(21*6, 0, 21, 32));
    textures["7"].loadFromFile(path, IntRect(21*7, 0, 21, 32));
    textures["8"].loadFromFile(path, IntRect(21*8, 0, 21, 32));
    textures["9"].loadFromFile(path, IntRect(21*9, 0, 21, 32));
    textures["-"].loadFromFile(path, IntRect(21*10, 0, 21, 32));
}

void TextureManager::Clear() {
    textures.clear();
}