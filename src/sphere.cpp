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

Sphere::Sphere(unsigned int ID, float radius)
    : m_shaderID(ID)
{
    setRadius(radius);
    buildVertices();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
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
}

void Sphere::setRotationVec(glm::vec3 vec)
{
    m_rotateVec = vec;
}

void Sphere::setRotationRad(float rad)
{
    m_rotRad = rad;
}

void Sphere::draw()
{
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    glBindVertexArray(m_VAO);

    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_transVec);
    m_model = glm::rotate(m_model, m_rotRad, m_rotateVec);

    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "model"), 1, GL_FALSE,
        &m_model[0][0]);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

// generate vertices with flat shading
// each triangle is independent (no shared vertices)
void Sphere::buildVertices()
{
    for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * std::numbers::pi) * std::sin(ySegment * std::numbers::pi);
            float yPos = std::cos(ySegment * std::numbers::pi);
            float zPos = std::sin(xSegment * 2.0f * std::numbers::pi) * std::sin(ySegment * std::numbers::pi);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        } else {
            for (int x = X_SEGMENTS; x >= 0; --x) {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount = static_cast<unsigned int>(indices.size());
    buildData();
}

void Sphere::buildData()
{
    for (unsigned int i = 0; i < positions.size(); ++i) {
        data.push_back(positions[i].x * m_radius);
        data.push_back(positions[i].y * m_radius);
        data.push_back(positions[i].z * m_radius);
        if (normals.size() > 0) {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
        if (uv.size() > 0) {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
    }
}

unsigned int Sphere::getShaderID()
{
    return m_shaderID;
}

glm::vec3* Sphere::getTranslate()
{
    return &m_transVec;
}

glm::mat4 Sphere::getModelMat()
{
    return m_model;
}
