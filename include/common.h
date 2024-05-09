#ifndef COMMON_H
#define COMMON_H
#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <iostream>
unsigned int loadTexture(char const* path);
const unsigned int SCR_WIDTH = 1240;
const unsigned int SCR_HEIGHT = 1080;
inline float timeMult = 1.0f;

#endif // !COMMON_H
