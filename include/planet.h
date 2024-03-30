#ifndef PLANET_H
#define PLANET_H
#include "include/sphere.h"

class Planet {
public:
    Planet(int distanceFromSun, float speedKmS, float rotationSpeedInHr, float radius, int subdivision);
    void draw();

private:
    Sphere m_sphere;
    float m_rotationSpeedInHr;
    float m_speedKmS;
    int m_distanceFromSun;
};
#endif // !
