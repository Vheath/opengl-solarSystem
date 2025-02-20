#ifndef SPHERE_H
#define SPHERE_H

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
class Sphere {
public:
    Sphere(unsigned int ShaderID, float radius);
    void draw();
    void SetShaderID(unsigned int ID);
    void setRadius(float radius);
    void setTranslate(glm::vec3 vec);
    void setRotationVec(glm::vec3 vec);
    void setRotationRad(float rad);
    void setTiltRad(float rad);

    unsigned int getShaderID();
    glm::mat4* getModelMat();
    glm::vec3* getTranslate();
    float getRadius();

    void buildData();
    void buildVertices();

private:
    unsigned int m_shaderID;
    unsigned int m_VBO,
        m_VAO,
        m_EBO;

    static const unsigned int X_SEGMENTS = 64;
    static const unsigned int Y_SEGMENTS = 64;
    static const unsigned int stride = (3 + 2 + 3) * sizeof(float);
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    unsigned int indexCount;
    std::vector<float> data;

    float m_radius;
    float m_rotRad {};
    float m_tiltRad {};

    glm::mat4 m_model = glm::mat4(1.0f);
    glm::vec3 m_transVec = glm::vec3(0.0f);
    glm::vec3 m_rotateVec = glm::vec3(1.0f);
};

#endif // !
