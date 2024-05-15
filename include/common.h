#ifndef COMMON_H
#define COMMON_H
#include "include/camera.h"
#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>
unsigned int loadTexture(char const* path);
const unsigned int SCR_WIDTH = 1240;
const unsigned int SCR_HEIGHT = 1080;
inline float timeMult = 1.0f;
inline Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
void genProjectionMatrix(float nearPlane, float farPlane);
inline glm::mat4 projectionMat;

#endif // !COMMON_H
