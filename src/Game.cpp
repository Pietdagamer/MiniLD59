#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace sf;

Game::Game(RenderWindow* _window)
{
    window = _window;
    loadTextures();
    //myTextureAtlas.setSmooth(true);
    windowWidth = 1280;
    windowWidth = 720;
    //player = Spaceship(Ship.PLAYER, Vector2f(windowWidth / 2, windowHeight / 2), 10);
}

void Game::update()
{
    Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
            {
                window->close();
            }
        }
        if (event.type == Event::Resized)
        {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            windowWidth = event.size.width;
            windowHeight = event.size.height;
            window->setView(View(visibleArea));
        }
    }
    frame++;
}

void Game::draw()
{
    window->clear();

    Sprite test;
    test.setTexture(myTextureAtlas);
    test.setPosition(Vector2f(256, 256));
    test.setOrigin(Vector2f(64, 64));
    //test.setRotation(frame / 1);
    test.setScale(Vector2f(2.0, 2.0));
    //std::cout << test.getRotation() << "\n";
    window->draw(test);

    //Draw the background
    Sprite background;
    background.setTexture(myTextureAtlas);
    for (int y = 0; y < int(windowHeight / 16.0) + 1; y++)
    {
        for (int x = 0; x < int(windowWidth / 16.0) + 1; x++)
        {
            background.setTextureRect(IntRect(randint(0, 7, (y + 1) * (x + 1)) * 16, 64, 16, 16));
            background.setPosition(x * 16, y * 16);
            window->draw(background);
        }
    }


    window->display();
}

void Game::loadTextures()
{
    if (!myTextureAtlas.loadFromFile("dontReplaceThis.png"))
        window->close();
    if (!someonesTextureAtlas.loadFromFile("replaceThis.png"))
        window->close();
}

int Game::randint(int low, int high, int seed)
{
    srand(seed);
    int value = rand() % (high + 1 - low) + low;
    return value;
}
