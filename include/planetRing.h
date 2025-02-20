#ifndef PLANETRING_H
#define PLANETRING_H

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "include/shader.h"
#include <glm/fwd.hpp>
#include <vector>

class PlanetRing {
public:
    PlanetRing(glm::vec3* translate, float radius, float lengthInRadius = 1, int sectors = 100);
    PlanetRing(float radius, float lengthInRadius = 1, int sectors = 400);
    PlanetRing();
    void render(Shader& shader);
    void initiateVertices();
    void initiateUVs();
    void initiateIndicies();
    void buildData();
    void setTranslate(glm::vec3* vec);
    void setRotationVec(glm::vec3 vec);
    void setRotationRad(float rad);
    void setModelMat(glm::mat4* model);

private:
    unsigned int m_VBO, m_VAO, m_EBO;
    float m_radiusPlanet;
    float m_lengthInRadius;
    int m_sectors;
    glm::vec3* m_translateVec;
    glm::mat4* m_model;
    std::vector<glm::vec3> m_vertices {};
    std::vector<glm::vec2> m_uvs {};
    std::vector<unsigned int> m_indicies {};
    std::vector<float> m_data;
};

#endif // !
