#ifndef PLANETRING_H
#define PLANETRING_H

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>

class PlanetRing {
public:
    PlanetRing(glm::vec3 center, float radiusm, int sectors = 100);
    void render();
    void initiateVertices();
    void initiateUVs();
    void initiateIndicies();
    void buildData();

private:
    unsigned int m_VBO, m_VAO, m_EBO;
    float m_radiusPlanet;
    int m_sectors;
    glm::vec3 m_centerOfPlanet;
    std::vector<glm::vec3> m_vertices {};
    std::vector<glm::vec2> m_uvs {};
    std::vector<unsigned int> m_indicies {};
    std::vector<float> m_data;
};

#endif // !
