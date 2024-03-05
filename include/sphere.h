#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
class Sphere {
public:
    Sphere(float radius, int subdivision);
    void draw() const;
    void icoNormVec(int i);
    void genNorm();
    void buildArr();
    void setRadius(float radius);
    void setEdgeLength(float edge);
    void setSubdivision(int subdivision);
    void updateRadius();
    std::vector<float> computeVertices();
    void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3]);
    void addLineIndices(unsigned int index);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    void addTexCoords(const float t1[2], const float t2[2], const float t3[2]);
    void addNormals(const float n1[3], const float n2[3], const float n3[3]);
    void addVertices(const float v1[3], const float v2[3], const float v3[3]);
    void buildInterleavedVertices();
    void buildVertices();
    void subdivideVertices();
    void computeHalfTexCoord(const float t1[2], const float t2[2], float newT[2]);
    void computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3]);

private:
    unsigned int m_VBO,
        m_normVBO,
        m_VAO;
    float m_radius;
    float m_edgeLength;

    int m_subdivision;

    std::vector<float> m_vertices;
    std::vector<float> m_interleavedVertices;
    std::vector<float> m_normals;
    std::vector<float> m_texCoords;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_lineIndices;
    constexpr static unsigned int m_tindices[20][3] = {
        { 1, 4, 0 }, { 4, 9, 0 }, { 4, 9, 5 }, { 8, 5, 4 }, { 1, 8, 4 },
        { 1, 10, 8 }, { 10, 3, 8 }, { 8, 3, 5 }, { 3, 2, 5 }, { 3, 7, 2 },
        { 3, 10, 7 }, { 10, 6, 7 }, { 6, 11, 7 }, { 6, 0, 11 }, { 6, 1, 0 },
        { 10, 1, 6 }, { 11, 0, 9 }, { 2, 11, 9 }, { 5, 2, 9 }, { 11, 2, 7 }
    };

    float m_n[3];
};

#endif // !
