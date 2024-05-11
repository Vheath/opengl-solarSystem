#include "include/planetRing.h"
#include <GL/gl.h>
#include <cmath>
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <numbers>

PlanetRing::PlanetRing(glm::vec3* center, float radius, int sectors)
    : m_centerOfPlanet { center }
    , m_radiusPlanet { radius }
    , m_sectors { 400 }
{
    if (center == nullptr) {
        static glm::vec3 nullVec { glm::vec3(0.0f) };
        m_centerOfPlanet = &nullVec;
    }
    initiateVertices();
    initiateUVs();
    initiateIndicies();
    buildData();
    initiateIndicies();
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicies.size() * sizeof(unsigned int), &m_indicies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), &m_data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

PlanetRing::PlanetRing(float radius, int sectors)
    : PlanetRing(nullptr, radius, sectors)
{
}
PlanetRing::PlanetRing()
    : PlanetRing(nullptr, 1.0f)
{
}

void PlanetRing::render()
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, 0);
}

void PlanetRing::setCenter(glm::vec3* vec)
{
    m_centerOfPlanet = vec;
    buildData();
}

void PlanetRing::buildData()
{
    m_data.clear();
    for (int i { 0 }; i < m_sectors * 3; ++i) {
        m_data.push_back(m_vertices[i].x);
        m_data.push_back(m_vertices[i].y);
        m_data.push_back(m_vertices[i].z);

        m_data.push_back(m_uvs[i].x);
        m_data.push_back(m_uvs[i].y);
    }
}
void PlanetRing::initiateVertices()
{
    m_vertices.clear();
    bool isEven { false };
    float twoPi { 2 * std::numbers::pi };

    for (int i { 0 }; i <= m_sectors; i += 2) {
        isEven = !isEven;

        float radianInStep { i * (twoPi / m_sectors) };
        float radianInStepNext { (i + 1) * (twoPi / m_sectors) };
        glm::vec3 xz1 {};
        glm::vec3 xz2 {};
        glm::vec3 xz3 {};
        if (isEven) {
            xz1.x = m_radiusPlanet * std::cos(radianInStep); // on sphere close
            xz1.z = m_radiusPlanet * std::sin(radianInStep);

            xz2.x = xz1.x + (m_radiusPlanet * std::cos(radianInStep)); // over sphere close
            xz2.z = xz1.z + (m_radiusPlanet * std::sin(radianInStep));

            xz3.x = m_radiusPlanet * std::cos(radianInStepNext); // on sphere next
            xz3.z = m_radiusPlanet * std::sin(radianInStepNext);
        } else {
            xz1.x = (m_radiusPlanet * std::cos(radianInStep)) + (m_radiusPlanet * std::cos(radianInStep)); // over sphere close
            xz1.z = (m_radiusPlanet * std::sin(radianInStep)) + (m_radiusPlanet * std::sin(radianInStep));

            xz2.x = m_radiusPlanet * std::cos(radianInStepNext); // on sphere next
            xz2.z = m_radiusPlanet * std::sin(radianInStepNext);

            xz3.x = xz2.x + (m_radiusPlanet * std::cos(radianInStepNext)); // over sphere next
            xz3.z = xz2.z + (m_radiusPlanet * std::sin(radianInStepNext));
        }

        m_vertices.push_back(xz1);
        m_vertices.push_back(xz2);
        m_vertices.push_back(xz3);
    }
}

void PlanetRing::initiateUVs()
{
    m_uvs.clear();
    for (int i { 0 }; i <= m_sectors / 2; ++i) {

        glm::vec2 uv1 = glm::vec2(0.0f, 0.0f + i % 2);
        glm::vec2 uv2 = glm::vec2(0.0f, 1.0f - i % 2);
        glm::vec2 uv3 = glm::vec2(1.0f, 0.0f + i % 2);

        m_uvs.push_back(uv1);
        m_uvs.push_back(uv2);
        m_uvs.push_back(uv3);
    }
}
void PlanetRing::initiateIndicies()
{
    m_indicies.clear();
    for (int i { 0 }; i < m_sectors * 3 / 2; ++i) {
        m_indicies.push_back(0 + i);
        m_indicies.push_back(1 + i);
        m_indicies.push_back(2 + i);
    }
}
