#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H
#include "Planet.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "dataLists.h"

using namespace sf;

enum StarType
{
    SUNLIKE,
    REDDWARF
};

class SolarSystem
{
    public:
        SolarSystem(Vector2f _position, dataLists* _dataptr);
        void update(Time dt);
        void draw(RenderWindow* window, Texture* texture, Color color);
        void generateSystem();
        void generateNear(int genSeed, int minPlanets, int maxPlanets);
        void generateGas(int genSeed);
        Vector2f getPosition() { return position; }
        std::vector<Planet>* getPlanets() { return &planets; }

    protected:
    private:
        std::vector<Planet> planets;
        Vector2f position;
        StarType starType = (StarType)0;

        int asteroidBelt = 0;               //0: no belt, > 0: radius of asteroid belt
        float asteroidRotation = 0.1;
        float asteroidSpeed = 0.1;

        dataLists* dataptr;


};

#endif // SOLARSYSTEM_H
