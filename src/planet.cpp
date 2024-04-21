#include "include/planet.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <numbers>

Planet::Planet(unsigned int shaderID, float distanceFromSun, int yearInEarthDays, float rotationTimeInHr, float radius, int subdivision)
    : m_distanceFromSun(distanceFromSun)
    , m_yearLength(yearInEarthDays)
    , m_rotationTimeInHr(rotationTimeInHr)
    , m_sphere(shaderID, radius, subdivision)
{
    m_sphere.setTranslate(glm::vec3(distanceFromSun, 0.0f, 0.0f));
    m_sphere.setRotationVec(glm::vec3(0.0f, 1.0f, 0.0f));
    m_yearSec = m_yearLength * 24 * 60 * 60; // 365 days(one earth year) / 2pi(one rotation)
    m_consYear = m_yearSec / (2.0f * std::numbers::pi);
}

glm::vec3* Planet::getTranslate()
{
    return m_sphere.getTranslate();
}

void Planet::draw()
{

    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    seconds += (deltaTime * timeMult) / m_consYear;

    seconds = std::fmod(seconds, std::numbers::pi * 2);

    rotationInSec += deltaTime * timeMult * 2 * std::numbers::pi / (m_rotationTimeInHr * 3600);
    m_sphere.setRotationRad(rotationInSec);

    m_sphere.setTranslate(glm::vec3(m_distanceFromSun * std::sin(seconds), 0.0f, m_distanceFromSun * std::cos(seconds)));
    m_sphere.draw();
}
