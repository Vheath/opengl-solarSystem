#include "include/planet.h"

Planet::Planet(int distanceFromSun, float speedKmS, float rotationSpeedInHr, float radius, int subdivision = 6)
    : m_distanceFromSun(distanceFromSun)
    , m_speedKmS(speedKmS)
    , m_rotationSpeedInHr(rotationSpeedInHr)
    , m_sphere(radius, subdivision)
{

}

