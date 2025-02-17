
#include "include/orbitOutline.h"
#include "include/common.h"
#include <GL/gl.h>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <numbers>
OrbitOutline::OrbitOutline(float radius, int sectors)
    : m_sectors { sectors }
    , m_radius { radius }
{
    m_shader.use();
    generateData();
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), &m_data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

OrbitOutline::OrbitOutline(int sectors)
    : OrbitOutline(0.0f, sectors)
{
}

void OrbitOutline::generateData()
{
    m_data.clear();
    float twoPi { 2 * std::numbers::pi };

    for (int i { 0 }; i < m_sectors; ++i) {
        float radianInStep { i * (twoPi / m_sectors) };
        glm::vec3 xz {};
        xz.x = std::cos(radianInStep) * m_radius;
        xz.z = std::sin(radianInStep) * m_radius;

        m_data.push_back(xz.x);
        m_data.push_back(xz.y);
        m_data.push_back(xz.z);
    }
}
void OrbitOutline::setRadius(float radius)
{
    m_radius = radius;
    generateData();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), &m_data[0], GL_STATIC_DRAW);
}

void OrbitOutline::draw()
{
    m_shader.use();
    glBindVertexArray(m_VAO);
    glm::mat4 model = glm::mat4(1.0f);
    m_shader.setMat4("model", model);
    m_shader.setMat4("view", camera.GetViewMatrix());
    m_shader.setMat4("projection", projectionMat);

    glDrawArrays(GL_LINE_LOOP, 0, m_data.size() / 3);
}
