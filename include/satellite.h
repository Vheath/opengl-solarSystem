#ifndef SATELLITE_H
#define SATELLITE_H

#include "include/common.h"
#include "include/sphere.h"
#include <glm/vec3.hpp>
#include <numbers>
class Satellite {
public:
    Satellite(unsigned int shaderID, glm::vec3* planetPos, float distanceInKm, float yearInEarthDays, float rotationTimeInHr, float radius, glm::vec3 color);
    void draw();

private:
    float m_distanceInKm {};
    float m_yearLength;
    float m_rotationTimeInHr;

    float currentFrame {};
    float deltaTime {};
    float lastFrame {};
    double seconds {};
    float rotationInSec {};
    float consYear = m_yearLength * 24 * 60 * 60 / (2 * std::numbers::pi); // 365 days(one earth year) / 2pi(one rotation)

    Sphere m_sphere;
    const glm::vec3* m_planetPos {};
    glm::vec3 m_color { 0.0f };
};

#endif
