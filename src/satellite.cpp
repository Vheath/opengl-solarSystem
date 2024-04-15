#include "include/satellite.h"
#include <GLFW/glfw3.h>
#include <numbers>

Satellite::Satellite(unsigned int shaderID, glm::vec3* planetPos,
    float distanceInKm, int yearInEarthDays, float rotationTimeInHr, float radius, glm::vec3 color)
    : m_planetPos(planetPos)
    , m_color(color)
    , m_distanceInKm(distanceInKm)
    , m_sphere(shaderID, radius, 5)
    , m_yearLength(yearInEarthDays)
    , m_rotationTimeInHr(rotationTimeInHr)
{
}

void Satellite::draw()
{
    glUniform3f(glGetUniformLocation(m_sphere.getShaderID(), "color"), m_color.x, m_color.y, m_color.z);

    static float currentFrame {};
    static float deltaTime {};
    static float lastFrame {};
    static double seconds {};
    static float rotationInSec {};
    static float consYear = m_yearLength * 24 * 60 * 60 / (2 * std::numbers::pi); // 365 days(one earth year) / 2pi(one rotation)

    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    seconds += (deltaTime * timeMult) / consYear;

    seconds = std::fmod(seconds, std::numbers::pi * 2);

    rotationInSec += deltaTime * timeMult * 2 * std::numbers::pi / (m_rotationTimeInHr * 3600);
    m_sphere.setRotationRad(rotationInSec);

    m_sphere.setTranslate(glm::vec3(m_planetPos->x + m_distanceInKm * std::sin(seconds), 0.0f, m_planetPos->z + m_distanceInKm * std::cos(seconds)));
    m_sphere.draw();
}
