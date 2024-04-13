#ifndef PLANET_H
#define PLANET_H
#include "include/sphere.h"

class Planet {
public:
    Planet(unsigned int ShaderID, int distanceFromSun, int yearInEarthDays, float rotationSpeedInHr, float radius, int subdivision = 4);
    void draw();
    void setTimeMultiplier(float timeMult); // more timeMult => faster earth rotation

private:
    Sphere m_sphere;
    float m_timeMult;
    float m_rotationSpeedInHr;
    int m_yearLength;
    int m_distanceFromSun;
};
#endif //
