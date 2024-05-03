#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H
#include "include/planet.h"
#include "include/satellite.h"
#include "include/shader.h"
#include <vector>

struct PlanetBody {
    Planet planet;
    unsigned int texture;
    unsigned int texGL;
    std::vector<Satellite> satVec;
};

enum PlanetNum {
    mercury = 0,
    venus,
    earth,
    mars,
    jupiter,
    saturn,
    uranus,
    neptune

};
class SolarSystem {

public:
    SolarSystem(std::string vertShader, std::string fragShader);
    std::vector<PlanetBody> planetVec;
    void initVec();
    void render();
    void render(Shader customShader);
    Shader shader;
    Shader sunShader { "../src/ShadersGLSL/sunShader.vert", "../src/ShadersGLSL/sunShader.frag" };

private:
    Sphere sun ;
    unsigned int diffuseSunMap;
    unsigned int diffuseMercuryMap;
    unsigned int diffuseVenusMap;
    unsigned int diffuseEarthMap;
    unsigned int diffuseMarsMap;
    unsigned int diffuseJupiterMap;
    unsigned int diffuseSaturnMap;
    unsigned int difuseUranusMap;
    unsigned int difuseNeptuneMap;
};
#endif // !SOLARSYSTEM_H
