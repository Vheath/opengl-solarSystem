#ifndef PLANET_H
#define PLANET_H
#include "include/common.h"
#include "include/sphere.h"

class Planet {
public:
    Planet(unsigned int ShaderID, float distanceFromSun, int yearInEarthDays, float rotationSpeedInHr, float radius, int subdivision = 4);
    void draw();

    glm::vec3* getTranslate();

private:
    Sphere m_sphere;
    float m_rotationTimeInHr;
    int m_yearLength;
    float m_distanceFromSun;
};
#endif //
