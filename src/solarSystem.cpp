#include "include/solarSystem.h"
#include "include/common.h"
#include "include/planetRing.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
unsigned int loadTextureRing(char const* path);

SolarSystem::SolarSystem(std::string vertShader, std::string fragShader)
    : shader { vertShader.c_str(), fragShader.c_str() }
{
    sun = Sphere { sunShader.ID, 1.0f };

    diffuseSunMap = loadTexture("../otherFiles/sunmap.png");
    diffuseMercuryMap = loadTexture("../otherFiles/mercurymap.jpeg");
    diffuseVenusMap = loadTexture("../otherFiles/venusmap.jpg");
    diffuseEarthMap = loadTexture("../otherFiles/earthmap.jpg");
    diffuseMarsMap = loadTexture("../otherFiles/marsmap.jpg");
    diffuseJupiterMap = loadTexture("../otherFiles/jupitermap.jpg");
    diffuseSaturnMap = loadTexture("../otherFiles/saturnmap.jpg");
    difuseUranusMap = loadTexture("../otherFiles/uranusmap.jpg");
    difuseNeptuneMap = loadTexture("../otherFiles/neptunemap.jpg");

    diffuseSaturnRingMap = loadTexture("../otherFiles/SaturnRingRGBA.png");
    initVec();

    saturnRing.setModelMat(planetVec[saturn].planet.m_sphere.getModelMat());
    planetVec[saturn].planet.m_sphere.setTiltRad(0.25f);
}

void SolarSystem::render()
{

    sunShader.use();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffuseSunMap);
    sunShader.setInt("material.diffuse", diffuseSunMap - 1);
    sun.draw();
    shader.use();
    for (int i { 0 }; i < planetVec.size(); ++i) {
        shader.use();
        shader.setBool("drawTexture", true);
        planetVec[i].planet.setShaderID(shader.ID);

        glActiveTexture(planetVec[i].texGL);
        glBindTexture(GL_TEXTURE_2D, planetVec[i].texture);
        shader.setInt("diffuseTexture", planetVec[i].texture - 1);
        planetVec[i].planet.draw();

        shader.setBool("drawTexture", false);
        for (int j { 0 }; j < planetVec[i].satVec.size(); ++j) {
            planetVec[i].satVec[j].draw();
        }
    }

    ringShader.use();
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, diffuseSaturnRingMap);
    ringShader.setInt("material.diffuse", diffuseSaturnRingMap - 1);
    saturnRing.render(ringShader);
}

void SolarSystem::render(Shader customShader)
{
    for (int i { 0 }; i < planetVec.size(); ++i) {
        customShader.use();
        customShader.setBool("drawTexture", true);
        planetVec[i].planet.setShaderID(customShader.ID);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, planetVec[i].texture);
        customShader.setInt("diffuseTexture", planetVec[i].texture - 1);
        planetVec[i].planet.draw();

        customShader.setBool("drawTexture", false);
        for (int j { 0 }; j < planetVec[i].satVec.size(); ++j) {
            planetVec[i].satVec[j].draw();
        }
    }
}

void SolarSystem::initVec()
{
    planetVec.push_back({ { shader.ID, 5.8f, 88, 59 * 24.0f, 0.24f },
        diffuseMercuryMap,
        GL_TEXTURE2 });

    // Venus third
    planetVec.push_back({ { shader.ID, 10.8f, 225, 243 * 24.0f, 0.60f },
        diffuseVenusMap,
        GL_TEXTURE3 });

    // Earth third
    planetVec.push_back({
        { shader.ID, 15, 365, 24.0f, 0.63f },
        diffuseEarthMap,
        GL_TEXTURE4,
    });

    // Mars fourth
    planetVec.push_back({ { shader.ID, 22.8f, 687, 24.8f, 0.34f },
        diffuseMarsMap,
        GL_TEXTURE5 });
    // Deimos and Phobos

    // Jupiter fifth 50(70) 3.21(7.21)
    planetVec.push_back({ { shader.ID, 50.8f, 4380, 9.8f, 3.21f },
        diffuseJupiterMap,
        GL_TEXTURE6 });

    // Saturn sixth
    planetVec.push_back({ { shader.ID, 80.8f, 10950, 10.5f, 3.0f },
        diffuseSaturnMap,
        GL_TEXTURE7 });

    // Uranus seventh (year 30k)
    planetVec.push_back({ { shader.ID, 100.0f, 20002, 17.0f, 1.0f },
        difuseUranusMap,
        GL_TEXTURE8 });

    // Neptune eighth (year 60k)
    planetVec.push_back({ { shader.ID, 130.0f, 24002, 16.0f, 0.95f },
        difuseNeptuneMap,
        GL_TEXTURE9 });

    planetVec[mars].satVec.push_back({ shader.ID, planetVec[mars].planet.getTranslate(), 1.3f, 1.264f, 30.2f, 0.067f, glm::vec3(0.4f) });
    planetVec[mars].satVec.push_back({ shader.ID, planetVec[mars].planet.getTranslate(), 0.6f, 0.31f, 7.6f, 0.087f, glm::vec3(0.4f) });

    planetVec[earth].satVec.push_back({ shader.ID, planetVec[earth].planet.getTranslate(), 1.5f, 27, 27 * 24, 0.087f, glm::vec3(0.4f) });
}
