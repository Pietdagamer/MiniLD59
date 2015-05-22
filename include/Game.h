#include <SFML/Graphics.hpp>
#include <iostream>
#include "Spaceship.h"
#include "Planet.h"
#include "SolarSystem.h"

#define GREEN Color(0, 200, 0)
#define WHITE Color(255, 255, 255)

using namespace sf;

class Game
{
    public:
        Game(RenderWindow* window);
        void update();
        void draw();
        void drawText();
        void drawBackground();
        void loadTextures();
        void drawTag(std::string text, Vector2f position, Color color);
        Time getTotalTime() { return totalTime; };
    protected:
    private:
        RenderWindow* window = nullptr;
        Texture myTextureAtlas;
        Texture someonesTextureAtlas;
        Texture fontTexture;
        int frame = 0;
        int windowWidth = 1280;
        int windowHeight = 720;
        Spaceship player {PLAYER, Vector2f(-1024, -1800), 400.0f};
        Clock clock;
        Time dt;
        Time totalTime;
        View view;
        bool radar = false;
        Color hudColor = radar ? GREEN : WHITE;

        std::vector <SolarSystem> systems;
};
