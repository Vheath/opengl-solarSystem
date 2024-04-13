#include "include/planet.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <numbers>

Planet::Planet(unsigned int shaderID, int distanceFromSun, int yearInEarthDays, float rotationTimeInHr, float radius, int subdivision)
    : m_distanceFromSun(distanceFromSun)
    , m_yearLength(yearInEarthDays)
    , m_rotationTimeInHr(rotationTimeInHr)
    , m_sphere(shaderID, radius, subdivision)
{
    m_sphere.setTranslate(glm::vec3(distanceFromSun, 0.0f, 0.0f));
    m_sphere.setRotationVec(glm::vec3(0.0f, 1.0f, 0.0f));
}

void Planet::setTimeMultiplier(float timeMult)
{
    m_timeMult = timeMult;
}

void Planet::draw()
{
    static float currentFrame {};
    static float deltaTime {};
    static float lastFrame {};
    static double seconds {};
    static float rotationInSec {};
    static float consYear = m_yearLength * 24 * 60 * 60 / (2 * std::numbers::pi); // 365 days(one earth year) / 2pi(one rotation)

    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    seconds += (deltaTime * m_timeMult) / consYear;

    seconds = std::fmod(seconds, std::numbers::pi * 2);

    glm::vec3 a = m_sphere.getTranslate();

    rotationInSec += deltaTime * m_timeMult * 2 * std::numbers::pi / (m_rotationTimeInHr * 3600);
     m_sphere.setRotationRad(rotationInSec);

    m_sphere.setTranslate(glm::vec3(m_distanceFromSun * std::sin(seconds), 0.0f, m_distanceFromSun * std::cos(seconds)));
    m_sphere.draw();
}
