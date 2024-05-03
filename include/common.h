#ifndef COMMON_H
#define COMMON_H
#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <iostream>
unsigned int loadTexture(char const* path);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
inline float timeMult = 1.0f;

#endif // !COMMON_H
