#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H
#include "include/planet.h"
#include "include/satellite.h"
#include "include/shader.h"
#include "include/planetRing.h"
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
    Shader ringShader {"../src/ShadersGLSL/PlanetRingShader.vert", "../src/ShadersGLSL/PlanetRingShader.frag"};

private:
    Sphere sun { 1, 1.0f };
    unsigned int diffuseSunMap;
    unsigned int diffuseMercuryMap;
    unsigned int diffuseVenusMap;
    unsigned int diffuseEarthMap;
    unsigned int diffuseMarsMap;
    unsigned int diffuseJupiterMap;
    unsigned int diffuseSaturnMap;
    unsigned int difuseUranusMap;
    unsigned int difuseNeptuneMap;
    unsigned int diffuseRingMap;
    PlanetRing saturnRing { glm::vec3(0.0f), 1.0f };
};
#endif // !SOLARSYSTEM_H
