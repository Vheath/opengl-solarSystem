#ifndef SATELLITE_H
#define SATELLITE_H

#include "include/common.h"
#include "include/sphere.h"
#include <glm/vec3.hpp>
class Satellite {
public:
    Satellite(unsigned int shaderID, glm::vec3* planetPos, float distanceInKm, int yearInEarthDays, float rotationTimeInHr, float radius, glm::vec3 color);
    void draw();

private:
    float m_distanceInKm {};
    float m_yearLength;
    float m_rotationTimeInHr;

    Sphere m_sphere;
    const glm::vec3* m_planetPos {};
    glm::vec3 m_color { 0.0f };
};

#endif
