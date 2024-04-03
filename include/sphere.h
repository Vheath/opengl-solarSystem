#ifndef SPHERE_H
#define SPHERE_H

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
class Sphere {
public:
    Sphere(unsigned int ShaderID, float radius, int subdivision = 6);
    void draw();
    void SetShaderID(unsigned int ID);
    void setRadius(float radius);
    void setEdgeLength(float edge);
    void setSubdivision(int subdivision);
    void setTranslate(glm::vec3 vec);
    void setRotationVec(glm::vec3 vec);
    void setRotationRad(float rad);

    unsigned int getShaderID();
    glm::mat4 getModelMat();
    glm::vec3 getTranslate();
    void updateRadius();
    std::vector<float> computeVertices();
    void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3]);
    void addLineIndices(unsigned int index);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    void addNormals(const float n1[3], const float n2[3], const float n3[3]);
    void addVertices(const float v1[3], const float v2[3], const float v3[3]);
    void buildInterleavedVertices();
    void buildVertices();
    void subdivideVertices();
    void computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3]);
    glm::vec2 generateTexCoord(const float nx, const float ny, const float nz);
    void icoNormVec(int i);
    void genNorm();
    void buildArr();
    int m_subdivision;

private:
    unsigned int m_shaderID;
    unsigned int m_VBO,
        m_VAO;
    float m_edgeLength;
    float m_radius;
    std::vector<float> m_vertices;
    std::vector<float> m_interleavedVertices;
    std::vector<float> m_normals;
    std::vector<unsigned int> m_indices;
    float m_rotRad {};

    glm::mat4 m_model = glm::mat4(1.0f);
    glm::vec3 m_transVec = glm::vec3(0.0f);
    glm::vec3 m_rotateVec = glm::vec3(1.0f);
};

#endif // !
