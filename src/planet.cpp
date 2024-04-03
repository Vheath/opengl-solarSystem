#include "include/planet.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <numbers>

Planet::Planet(unsigned int shaderID, int distanceFromSun, float speedKmS, float rotationSpeedInHr, float radius, int subdivision)
    : m_distanceFromSun(distanceFromSun)
    , m_speedKmS(speedKmS)
    , m_rotationSpeedInHr(rotationSpeedInHr)
    , m_sphere(shaderID, radius, subdivision)
{
    m_sphere.setTranslate(glm::vec3(distanceFromSun, 0.0f, 0.0f));
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
    static float consYear = 49536632.9618f; // 365 days(one earth year) / 2pi(one rotation)
    
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    seconds += (deltaTime * m_timeMult) / consYear;

    seconds = std::fmod(seconds, std::numbers::pi * 2);

    glm::vec3 a = m_sphere.getTranslate();

    m_sphere.setTranslate(glm::vec3(m_distanceFromSun * std::sin(seconds), 0.0f, m_distanceFromSun * std::cos(seconds)));
    m_sphere.draw();
}
