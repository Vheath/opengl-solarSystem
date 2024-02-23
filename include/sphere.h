#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
class Sphere {
public:
    Sphere(float radius);
    void draw() const;
    void icoNormVec(int i);
    void genNorm();
    void buildArr();
    void setRadius(float radius);
    void setEdgeLength(float edge);
    void updateRadius();
    std::vector<float> computeVertices();
    void computeFaceNormal(float v1[3], float v2[3], float v3[3], float n[3]);
    void addLineIndices(unsigned int index);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    void addTexCoords(float t1[2], float t2[2], float t3[2]);
    void addNormals(float n1[3], float n2[3], float n3[3]);
    void addVertices(float v1[3], float v2[3], float v3[3]);
    void buildInterleavedVertices();
    void buildVertices();

private:
    unsigned int VBO,
        normVBO,
        VAO;
    float radius;
    float edgeLength;
    constexpr static float X { 0.525731112119133606f };
    constexpr static float Z { 0.850650808352039932f };

    constexpr static float vdata[12][3] {
        { -X, 0, Z }, { X, 0, Z }, { -X, 0, -Z }, { X, 0, -Z },
        { 0, Z, X }, { 0, Z, -X }, { 0, -Z, X }, { 0, -Z, -X },
        { Z, X, 0 }, { -Z, X, 0 }, { Z, -X, 0 }, { -Z, -X, 0 }
    };

    std::vector<float> vertices;
    std::vector<float> interleavedVertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    constexpr static unsigned int tindices[20][3] = {
        { 1, 4, 0 }, { 4, 9, 0 }, { 4, 9, 5 }, { 8, 5, 4 }, { 1, 8, 4 },
        { 1, 10, 8 }, { 10, 3, 8 }, { 8, 3, 5 }, { 3, 2, 5 }, { 3, 7, 2 },
        { 3, 10, 7 }, { 10, 6, 7 }, { 6, 11, 7 }, { 6, 0, 11 }, { 6, 1, 0 },
        { 10, 1, 6 }, { 11, 0, 9 }, { 2, 11, 9 }, { 5, 2, 9 }, { 11, 2, 7 }
    };

    float n[3];
};

#endif // !
