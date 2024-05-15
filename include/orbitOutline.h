#ifndef ORBITOUTLINE_H
#define ORBITOUTLINE_H

#include "include/shader.h"
#include <vector>
class OrbitOutline {
public:
    OrbitOutline(float radius, int sectors = 100);
    OrbitOutline(int sectors = 100);
    void setRadius(float radius);
    void generateData();


    void draw();

private:
    unsigned int m_VAO, m_VBO;
    float m_radius;
    int m_sectors;
    std::vector<float> m_data;
    Shader m_shader { "../src/ShadersGLSL/orbitShader.vert", "../src/ShadersGLSL/orbitShader.frag" };
};

#endif // !
