#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>
#include <string>
using namespace std;
using namespace sf;

class TextureManager {

    static unordered_map<string, Texture> textures;
public:
    static void LoadTexture(string name);
    static Texture& GetTexture(string name);
    static void SetDigits();
    static void Clear();

};