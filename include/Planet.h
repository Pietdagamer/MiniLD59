#ifndef PLANET_H
#define PLANET_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "dataLists.h"

using namespace sf;

int randint(int low, int high);

enum PlanetName
{
    MERCURIUS,
    VENUS,
    EARTH,
    MARS,
    PLUTO,
    JUPITER,
    SATURN,
    NEPTUNE
};

class Planet : public GameObject
{
    public:
        Planet(float _circulationSpeed, float _radius, PlanetName _planetName, dataLists* _dataptr, Vector2f _systemPos);
        void draw(RenderWindow* window, Texture* texture, Color color);
        void update(Time dt);
        void setPosition(Vector2f value) { position = value; }
        float getRadius() { return radius; }
        float getCirculationPosition() { return circulationPosition; }
        std::string generatePlanetEntry();
        std::string generatePlanetName(std::vector<std::string>* _baseNames);
        std::string getName() { return name; }
        std::string getEntry() { return entry; }
        Vector2f getPosition() { return position; }
    protected:
    private:
        Vector2f systemPosition; //Used in the seed for random text/name generation
        Vector2f position;
        PlanetName planetName = (PlanetName)0;

        //These values determine the traffic from and to the planet
        int pullFactor = 0;
        int pushFactor = 0;

        float radius = 0;
        float circulationSpeed = 0.1; //radians per second
        float circulationPosition = randint(0, 3);

        Vector2f spritePositions[8] =  {Vector2f(64, 96),  //Mercurius
                                        Vector2f(80, 80),  //Venus
                                        Vector2f(64, 80),  //Earth
                                        Vector2f(48, 96),  //Mars
                                        Vector2f(96, 80),  //Pluto
                                        Vector2f(48, 80),  //Jupiter
                                        Vector2f(32, 80),  //Saturn
                                        Vector2f(32, 96)}; //Neptune

        int genSeed = 0;

        std::string name = "NoNameAssigned";
        std::string entry = "NoEntryAssigned";
        dataLists* dataptr = nullptr;

};


#endif // PLANET_H
