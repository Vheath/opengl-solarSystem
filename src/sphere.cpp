#include <glad/glad.h>

#include "include/sphere.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <math.h>
#include <numbers>

Sphere::Sphere(unsigned int ID, float radius, int subdivision)
    : m_subdivision(subdivision)
    , m_shaderID(ID)
{
    setRadius(radius);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
}

// setters
//
void Sphere::SetShaderID(unsigned int ID)
{
    m_shaderID = ID;
}

void Sphere::setTranslate(glm::vec3 vec)
{
    m_transVec = vec;
}

void Sphere::setRadius(float radius)
{
    m_radius = radius;
    m_edgeLength = radius / sinf(2 * 3.141592f / 5);
    if (m_vertices.size() <= 0)
        buildVertices();
    else
        updateRadius(); // update vertex positions only
}

void Sphere::setEdgeLength(float edge)
{
    m_edgeLength = edge;
    m_radius = edge * sinf(2 * 3.141592f / 5);
    updateRadius(); // update vertex positions only
}

void Sphere::setSubdivision(int subdivision)
{
    m_subdivision = subdivision;
    buildVertices();
}

void Sphere::setRotationVec(glm::vec3 vec)
{
    m_rotateVec = vec;
}

void Sphere::setRotationRad(float rad)
{
    m_rotRad = rad;
}

// draw an icosahedron
void Sphere::draw()
{

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_interleavedVertices.size() * sizeof(float), &m_interleavedVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)+(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)+(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    float angle = glfwGetTime();

    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_transVec);
    m_model = glm::rotate(m_model, m_rotRad, m_rotateVec);

    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "model"), 1, GL_FALSE,
        &m_model[0][0]);

    glDrawElements(GL_TRIANGLES, (unsigned int)m_indices.size(), GL_UNSIGNED_INT, m_indices.data());
}

// update vertex positions only
void Sphere::updateRadius()
{
    float scale = sqrtf(m_radius * m_radius / (m_vertices[0] * m_vertices[0] + m_vertices[1] * m_vertices[1] + m_vertices[2] * m_vertices[2]));

    std::size_t i, j;
    std::size_t count = m_vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 8) {
        m_vertices[i] *= scale;
        m_vertices[i + 1] *= scale;
        m_vertices[i + 2] *= scale;

        // for interleaved array
        m_interleavedVertices[j] *= scale;
        m_interleavedVertices[j + 1] *= scale;
        m_interleavedVertices[j + 2] *= scale;
    }
}

// compute 12 vertices of icosahedron using spherical coordinates
// The north pole is at (0, 0, r) and the south pole is at (0,0,-r).
// 5 vertices are placed by rotating 72 deg at elevation 26.57 deg (=atan(1/2))
// 5 vertices are placed by rotating 72 deg at elevation -26.57 deg
std::vector<float> Sphere::computeVertices()
{
    const float PI = 3.1415926f;
    const float H_ANGLE = PI / 180 * 72; // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2); // elevation = 26.565 degree

    std::vector<float> vertices(12 * 3); // 12 vertices
    int i1, i2; // indices
    float z, xy; // coords
    float hAngle1 = -PI / 2 - H_ANGLE / 2; // start from -126 deg at 2nd row
    float hAngle2 = -PI / 2; // start from -90 deg at 3rd row

    // the first top vertex (0, 0, r)
    vertices[0] = 0;
    vertices[1] = 0;
    vertices[2] = m_radius;

    // 10 vertices at 2nd and 3rd rows
    for (int i = 1; i <= 5; ++i) {
        i1 = i * 3; // for 2nd row
        i2 = (i + 5) * 3; // for 3rd row

        z = m_radius * sinf(V_ANGLE); // elevaton
        xy = m_radius * cosf(V_ANGLE);

        vertices[i1] = xy * cosf(hAngle1); // x
        vertices[i2] = xy * cosf(hAngle2);
        vertices[i1 + 1] = xy * sinf(hAngle1); // y
        vertices[i2 + 1] = xy * sinf(hAngle2);
        vertices[i1 + 2] = z; // z
        vertices[i2 + 2] = -z;

        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;
    }

    // the last bottom vertex (0, 0, -r)
    i1 = 11 * 3;
    vertices[i1] = 0;
    vertices[i1 + 1] = 0;
    vertices[i1 + 2] = -m_radius;

    return vertices;
}

// generate vertices with flat shading
// each triangle is independent (no shared vertices)
void Sphere::buildVertices()
{
    // const float S_STEP = 1 / 11.0f;         // horizontal texture step
    // const float T_STEP = 1 / 3.0f;          // vertical texture step
    const float S_STEP = 186 / 2048.0f; // horizontal texture step
    const float T_STEP = 322 / 1024.0f; // vertical texture step

    // compute 12 vertices of icosahedron
    std::vector<float> tmpVertices = computeVertices();

    // clear memory of prev arrays
    std::vector<float>().swap(m_vertices);
    std::vector<float>().swap(m_normals);
    std::vector<unsigned int>().swap(m_indices);
    float *v0, *v1, *v2, *v3, *v4, *v11; // vertex positions
    float n[3]; // face normal
    float t0[2], t1[2], t2[2], t3[2], t4[2], t11[2]; // texCoords
    unsigned int index = 0;

    // compute and add 20 tiangles
    v0 = &tmpVertices[0]; // 1st vertex
    v11 = &tmpVertices[11 * 3]; // 12th vertex
    for (int i = 1; i <= 5; ++i) {
        // 4 vertices in the 2nd row
        v1 = &tmpVertices[i * 3];
        if (i < 5)
            v2 = &tmpVertices[(i + 1) * 3];
        else
            v2 = &tmpVertices[3];

        v3 = &tmpVertices[(i + 5) * 3];
        if ((i + 5) < 10)
            v4 = &tmpVertices[(i + 6) * 3];
        else
            v4 = &tmpVertices[6 * 3];

        // texture coords
        t0[0] = (2 * i - 1) * S_STEP;
        t0[1] = 0;
        t1[0] = (2 * i - 2) * S_STEP;
        t1[1] = T_STEP;
        t2[0] = (2 * i - 0) * S_STEP;
        t2[1] = T_STEP;
        t3[0] = (2 * i - 1) * S_STEP;
        t3[1] = T_STEP * 2;
        t4[0] = (2 * i + 1) * S_STEP;
        t4[1] = T_STEP * 2;
        t11[0] = 2 * i * S_STEP;
        t11[1] = T_STEP * 3;

        // add a triangle in 1st row
        Sphere::computeFaceNormal(v0, v1, v2, n);
        addVertices(v0, v1, v2);
        addNormals(n, n, n);
        addIndices(index, index + 1, index + 2);

        // add 2 triangles in 2nd row
        Sphere::computeFaceNormal(v1, v3, v2, n);
        addVertices(v1, v3, v2);
        addNormals(n, n, n);
        addIndices(index + 3, index + 4, index + 5);

        Sphere::computeFaceNormal(v2, v3, v4, n);
        addVertices(v2, v3, v4);
        addNormals(n, n, n);
        addIndices(index + 6, index + 7, index + 8);

        // add a triangle in 3rd row
        Sphere::computeFaceNormal(v3, v11, v4, n);
        addVertices(v3, v11, v4);
        addNormals(n, n, n);
        addIndices(index + 9, index + 10, index + 11);

        // next index
        index += 12;
    }

    subdivideVertices();
    // generate interleaved vertex array as well
    buildInterleavedVertices();
}

// generate interleaved vertices: V/N/T
// stride must be 32 bytes
void Sphere::buildInterleavedVertices()
{
    std::vector<float>().swap(m_interleavedVertices);

    std::size_t i, j;
    std::size_t count = m_vertices.size();
    for (i = 0, j = 0; i < count; i += 3, j += 2) {
        m_interleavedVertices.push_back(m_vertices[i]);
        m_interleavedVertices.push_back(m_vertices[i + 1]);
        m_interleavedVertices.push_back(m_vertices[i + 2]);

        m_interleavedVertices.push_back(m_normals[i]);
        m_interleavedVertices.push_back(m_normals[i + 1]);
        m_interleavedVertices.push_back(m_normals[i + 2]);

        m_interleavedVertices.push_back(generateTexCoord(m_normals[i], m_normals[i + 1], m_normals[i + 2]).x);
        m_interleavedVertices.push_back(generateTexCoord(m_normals[i], m_normals[i + 1], m_normals[i + 2]).y);
    }
}

// add 3 vertices to array
void Sphere::addVertices(const float v1[3], const float v2[3], const float v3[3])
{
    m_vertices.push_back(v1[0]); // x
    m_vertices.push_back(v1[1]); // y
    m_vertices.push_back(v1[2]); // z
    m_vertices.push_back(v2[0]);
    m_vertices.push_back(v2[1]);
    m_vertices.push_back(v2[2]);
    m_vertices.push_back(v3[0]);
    m_vertices.push_back(v3[1]);
    m_vertices.push_back(v3[2]);
}

// add 3 normals to array
void Sphere::addNormals(const float n1[3], const float n2[3], const float n3[3])
{
    m_normals.push_back(n1[0]); // nx
    m_normals.push_back(n1[1]); // ny
    m_normals.push_back(n1[2]); // nz
    m_normals.push_back(n2[0]);
    m_normals.push_back(n2[1]);
    m_normals.push_back(n2[2]);
    m_normals.push_back(n3[0]);
    m_normals.push_back(n3[1]);
    m_normals.push_back(n3[2]);
}

// geneating texture cord from normal
glm::vec2 Sphere::generateTexCoord(const float nx, const float ny, const float nz)
{
    float theta = (atan2f(nx, nz) / std::numbers::pi) / 2.0f + 0.5f;
    float phi = (asinf(-ny) / (std::numbers::pi / 2.0f)) / 2.0f + 0.5f;
    return glm::vec2(theta, phi);
}

// add 3 indices to array
void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    m_indices.push_back(i1);
    m_indices.push_back(i2);
    m_indices.push_back(i3);
}

// return face normal of a triangle v1-v2-v3
// if a triangle has no surface (normal length = 0), then return a zero vector
void Sphere::computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3])
{

    // default return value (0, 0, 0)
    n[0] = n[1] = n[2] = 0;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = v2[0] - v1[0];
    float ey1 = v2[1] - v1[1];
    float ez1 = v2[2] - v1[2];
    float ex2 = v3[0] - v1[0];
    float ey2 = v3[1] - v1[1];
    float ez2 = v3[2] - v1[2];

    // cross product: e1 x e2
    float nx, ny, nz;
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > 0) {
        // normalize
        float lengthInv = 1.0f / length;
        n[0] = nx * lengthInv;
        n[1] = ny * lengthInv;
        n[2] = nz * lengthInv;
    }
}

void Sphere::subdivideVertices()
{
    std::vector<float> tmpVertices;
    std::vector<unsigned int> tmpIndices;
    int indexCount;
    const float *v1, *v2, *v3; // ptr to original vertices of a triangle
    const float *t1, *t2, *t3; // ptr to original texcoords of a triangle
    float newV1[3], newV2[3], newV3[3]; // new vertex positions
    float normal[3]; // new face normal
    unsigned int index = 0; // new index value
    int i, j;

    // iteration
    for (i = 1; i <= m_subdivision; ++i) {
        // copy prev arrays
        tmpVertices = m_vertices;
        tmpIndices = m_indices;

        // clear prev arrays
        m_vertices.clear();
        m_normals.clear();
        m_indices.clear();

        index = 0;
        indexCount = (int)tmpIndices.size();
        for (j = 0; j < indexCount; j += 3) {
            // get 3 vertice and texcoords of a triangle
            v1 = &tmpVertices[tmpIndices[j] * 3];
            v2 = &tmpVertices[tmpIndices[j + 1] * 3];
            v3 = &tmpVertices[tmpIndices[j + 2] * 3];

            // get 3 new vertices by spliting half on each edge
            computeHalfVertex(v1, v2, m_radius, newV1);
            computeHalfVertex(v2, v3, m_radius, newV2);
            computeHalfVertex(v1, v3, m_radius, newV3);

            // add 4 new triangles
            addVertices(v1, newV1, newV3);
            computeFaceNormal(v1, newV1, newV3, normal);
            addNormals(normal, normal, normal);
            addIndices(index, index + 1, index + 2);

            addVertices(newV1, v2, newV2);
            computeFaceNormal(newV1, v2, newV2, normal);
            addNormals(normal, normal, normal);
            addIndices(index + 3, index + 4, index + 5);

            addVertices(newV1, newV2, newV3);
            computeFaceNormal(newV1, newV2, newV3, normal);
            addNormals(normal, normal, normal);
            addIndices(index + 6, index + 7, index + 8);

            addVertices(newV3, newV2, v3);
            computeFaceNormal(newV3, newV2, v3, normal);
            addNormals(normal, normal, normal);
            addIndices(index + 9, index + 10, index + 11);

            // next index
            index += 12;
        }
    }
}

void Sphere::computeHalfVertex(const float v1[3], const float v2[3], float length, float newV[3])
{
    newV[0] = v1[0] + v2[0];
    newV[1] = v1[1] + v2[1];
    newV[2] = v1[2] + v2[2];
    float scale = length / sqrtf(newV[0] * newV[0] + newV[1] * newV[1] + newV[2] * newV[2]); // compute scale factor to resize vector
    newV[0] *= scale;
    newV[1] *= scale;
    newV[2] *= scale;
}

unsigned int Sphere::getShaderID()
{
    return m_shaderID;
}

glm::vec3 Sphere::getTranslate()
{
    return m_transVec;
}

glm::mat4 Sphere::getModelMat()
{
    return m_model;
}
