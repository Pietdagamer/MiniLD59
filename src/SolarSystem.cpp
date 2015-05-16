#include "SolarSystem.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

#define TO_DEGREES_MULT 5729577951

using namespace sf;

void celShade(Sprite sprite, RenderWindow* window, Color shadeColor);
int randint(int low, int high, int seed);
int randint(int low, int high);


SolarSystem::SolarSystem(Vector2f _position)
{
    position = _position;
    generateSystem();
}

void SolarSystem::update(Time dt)
{
    for (int i = 0; i < planets.size(); i++)
    {
        planets.at(i).update(dt);
    }
}

void SolarSystem::draw(RenderWindow* window, Texture* texture)
{
    Sprite starSprite;
    starSprite.setTexture(*texture);

    if (starType == REDDWARF)
    {
        starSprite.setTextureRect(IntRect(80, 96, 16, 16));
        starSprite.setOrigin(Vector2f(8, 8));
        starSprite.setPosition(position);
    }
    else if (starType == SUNLIKE)
    {
        starSprite.setTextureRect(IntRect(0, 80, 32, 32));
        starSprite.setOrigin(Vector2f(16, 16));
        starSprite.setPosition(position);
    }

    for (int i = 0; i < planets.size(); i++)
    {
        Vector2f planetPos;
        float angle = planets.at(i).getCirculationPosition();
        float radius = planets.at(i).getRadius();
        planetPos.x = (cos((float)angle)) * (float)radius;
        planetPos.y = (sin((float)angle)) * (float)radius;

        planets.at(i).setPosition(planetPos + position);

        planets.at(i).draw(window, texture);
    }

    celShade(starSprite, window, Color(0, 0, 0));
    window->draw(starSprite);
}

void SolarSystem::generateSystem()
{
    int genSeed = position.x * position.y;

    int randomStar = randint(0, 1000, genSeed*2);

    if (randomStar > 500)
        starType = REDDWARF;
    else
        starType = SUNLIKE;

    if (starType == REDDWARF)
    {
        generateNear(genSeed);

        if (randint(0, 3, genSeed) != 1) //75% percent change on asteroid belt
            asteroidBelt = planets.back().getRadius() + randint(15, 50, genSeed);

        //std::cout << "Red dwarf system, " << amountOfNearPlanets << " near planets.\n";
    }
    else if (starType == SUNLIKE)
    {
        generateNear(genSeed);
        generateGas(genSeed);

        if (randint(0, 3, genSeed) == 1) //25% percent change on asteroid belt
            asteroidBelt = planets.back().getRadius() + randint(50, 100), genSeed;

        //std::cout << "Sunlike system, " << amountOfNearPlanets << " near planets, " << amountOfGasPlanets << " gas planets.\n";
    }
}

void SolarSystem::generateNear(int genSeed)
{
    int amountOfNearPlanets = randint(2, 4, genSeed*3);
    for (int i = 1; i <= amountOfNearPlanets; i++)
    {
        planets.push_back(Planet(randint(2500, 8000, genSeed*i*4) / 5000.0f, i * randint(15, 95, genSeed*i*5), (PlanetName)(randint(0, 4, genSeed*i*7)), 1, 1));
    }

}

void SolarSystem::generateGas(int genSeed)
{
    int amountOfGasPlanets = randint(1, 4, genSeed);
    int amountOfNearPlanets = planets.size();
    for (int i = 1; i <= amountOfGasPlanets; i++)
    {
        planets.push_back(Planet(randint(100, 600, genSeed*i*4) / 5000.0f, planets.at(amountOfNearPlanets - 1).getRadius() + i * randint(105, 195, genSeed*i*5), (PlanetName)(randint(5, 7, genSeed*i*6)), 1, 1));
    }
}
