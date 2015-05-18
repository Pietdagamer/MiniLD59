#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace sf;

void celShade(Sprite sprite, RenderWindow* window, Color shadeColor);
int randint(int low, int high, int seed);
void drawString(RenderWindow* window, std::string text, Vector2f position, Texture* fontTexture, Color color);

Game::Game(RenderWindow* _window)
{
    window = _window;
    loadTextures();
    //myTextureAtlas.setSmooth(true);
}

void Game::update()
{
    //Events
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

    //Time
    dt = clock.restart();
    totalTime += dt;

    //Create systems
    int renderDistance = 1000;
    Vector2f     topLeftVisible(view.getCenter().x - windowWidth / 2 - renderDistance, view.getCenter().y - windowHeight / 2 - renderDistance);
    Vector2f bottomRightVisible(view.getCenter().x + windowWidth / 2 + renderDistance, view.getCenter().y + windowHeight / 2 + renderDistance);
    Vector2f topLeftSystemTile;
    topLeftSystemTile.x = ((int)topLeftVisible.x - (int)topLeftVisible.x % 512) / 512;
    topLeftSystemTile.y = ((int)topLeftVisible.y - (int)topLeftVisible.y % 512) / 512;
    Vector2f bottomRightSystemTile;
    bottomRightSystemTile.x = ((int)bottomRightVisible.x - (int)bottomRightVisible.x % 512) / 512;
    bottomRightSystemTile.y = ((int)bottomRightVisible.y - (int)bottomRightVisible.y % 512) / 512;

    for (int y = topLeftSystemTile.y; y < bottomRightSystemTile.y; y++)
    {
        for (int x = topLeftSystemTile.x; x < bottomRightSystemTile.x; x++)
        {
            if (randint(0, 13, x * y) == 0)
            {
                Vector2f newSystemPosition;
                newSystemPosition.x = x * 512 + randint(-256, 256, x * y);
                newSystemPosition.y = y * 512 + randint(-256, 256, x ^ y);

                bool locationAvailable = true;
                for (int i = 0; i < systems.size(); i++)
                {
                    if (systems.at(i).getPosition() == newSystemPosition)
                    {
                        locationAvailable = false;
                        break;
                    }
                    else
                    {
                        continue;
                    }

                }
                if (locationAvailable)
                    systems.push_back(SolarSystem(Vector2f(newSystemPosition.x, newSystemPosition.y)));
            }
        }
    }

    //Delete systems
    Vector2f topLeftDestroy = topLeftVisible;
    topLeftDestroy.x -= 250;
    topLeftDestroy.y -= 250;
    Vector2f bottomRightDestroy = bottomRightVisible;
    bottomRightDestroy.x -= 250;
    bottomRightDestroy.y -= 250;

    for (int i = systems.size() - 1; i >= 0; i--)
    {
        Vector2f systemPos = systems.at(i).getPosition();
        if (!((systemPos.x > topLeftDestroy.x && systemPos.y > topLeftDestroy.y) && (systemPos.x < bottomRightDestroy.x && systemPos.y < bottomRightDestroy.y)))
        {
            systems.erase(systems.begin() + i);
        }
    }


    if (Keyboard::isKeyPressed(Keyboard::F3))
        std::cout << "SysNum:" << systems.size() << ", FPS:" << 1.0 / dt.asSeconds() << ", X:" << player.getPosition().x << ", Y:" << player.getPosition().y
                  << ", dstX:" << topLeftDestroy.x << ", dstY:" << topLeftDestroy.y << "\n";

    //screenshots!
    if (Keyboard::isKeyPressed(Keyboard::F11))
    {
        Image Screen = window->capture();
        Screen.saveToFile("screenshot.png");
    }


    //Update objects
    player.update(dt);
    //testSystem.update(dt);
    for (int i = 0; i < systems.size(); i++)
    {
        systems.at(i).update(dt);
    }

    frame++;
}

void Game::draw()
{
    window->clear(Color(255, 255, 255));

    //Set the view
    Vector2f roundedPosition;
    roundedPosition.x = (int)player.getPosition().x;
    roundedPosition.y = (int)player.getPosition().y;

    view.setCenter(roundedPosition);
    view.setSize(windowWidth, windowHeight);

    if (Joystick::isButtonPressed(0, 0) || Keyboard::isKeyPressed(Keyboard::Comma))
    {
        view.zoom(0.5);
    }
    if (Joystick::isButtonPressed(0, 1) || Keyboard::isKeyPressed(Keyboard::Period))
    {
        view.zoom(0.125);
    }

    window->setView(view);


    //Draw the background
    Sprite background;
    background.setTexture(myTextureAtlas);
    Vector2f topLeftTile;// = player.getPosition() - (Vector2f(windowWidth, windowHeight) / 2.0f);

    topLeftTile.x = (int)(player.getPosition().x - windowWidth / 2.0f);
    topLeftTile.x = topLeftTile.x - (int)topLeftTile.x % 16 - 16;

    topLeftTile.y = (int)(player.getPosition().y - windowHeight / 2.0f);
    topLeftTile.y = topLeftTile.y - (int)topLeftTile.y % 16 - 16;

    for (int y = 0; y < (int)(windowHeight / 16.0) + 3; y++)
    {
        for (int x = 0; x < (int)(windowWidth / 16.0) + 3; x++)
        {
            background.setTextureRect(IntRect(randint(0, 7, (y + 420 + topLeftTile.y / 16) * (x + 420 + topLeftTile.x / 16)) * 16, 64, 16, 16));
            background.setPosition(x * 16 + topLeftTile.x, y * 16 + topLeftTile.y);
            window->draw(background);
        }
    }

    //Draw all the objects
    //testSystem.draw(window, &myTextureAtlas);
    for (int i = 0; i < systems.size(); i++)
    {
        systems.at(i).draw(window, &myTextureAtlas);
    }
    player.draw(window, &myTextureAtlas);
    //testPlanet.draw(window, &myTextureAtlas);

    drawText();

    window->display();
}

void Game::drawText()
{
    for (int i = 0; i < systems.size(); i++)
    {
        std::string text = "X:" + std::to_string((int)systems.at(i).getPosition().x) + ", Y:" + std::to_string((int)systems.at(i).getPosition().y);
        Vector2f drawPos(systems.at(i).getPosition().x + 25, systems.at(i).getPosition().y - 25);
        drawTag(text, drawPos, Color(0, 200, 0));
        
        std::vector<Planet>* planets = systems.at(i).getPlanets();
        for (int j = 0; j < planets->size(); j++)
        {
            std::string text = "X:" + std::to_string((int)planets->at(j).getPosition().x + 
                               ", Y:" + std::to_string((int)planets->at(j)).getPosition().y;
            vector2f drawPos(planets->getPosition().x + 10, planets->getPosition().y - 10);
            drawTag(text, drawPos, Color(0, 200, 0));
        }
    }

    std::string text = "X:" + std::to_string((int)player.getPosition().x) + ", Y:" + std::to_string((int)player.getPosition().y);
    Vector2f drawPos(player.getPosition().x + 12, player.getPosition().y - 12);
    drawTag(text, drawPos, Color(0, 200, 0));
}

void Game::loadTextures()
{
    if (!myTextureAtlas.loadFromFile("dontReplaceThis.png"))
        window->close();
    if (!someonesTextureAtlas.loadFromFile("replaceThis.png"))
        window->close();
    if (!fontTexture.loadFromFile("font.png"))
        window->close();
}

void Game::drawTag(std::string text, Vector2f position, Color color)
{
    RectangleShape bg(Vector2f(text.length() * 6 + 3, 4));
    bg.setPosition(Vector2f(position.x - 2, position.y - 2));
    bg.setOutlineThickness(1);
    bg.setOutlineColor(color);
    bg.setFillColor(Color(0, 0, 0));
    window->draw(bg);
    
    drawString(window, text, position, &fontTexture, color);
}
