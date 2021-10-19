#ifndef NOISE_H
#define NOISE_H

#include "math.h"

#include <glm.hpp>
#include <gtc/quaternion.hpp>

#include <vector>

float srandNormalized(unsigned int seed);

glm::vec3 hash3(glm::vec3 point);
glm::vec3 hash3(glm::vec2 point);
glm::vec2 hash2(glm::vec2 point);
float hash1(float point);

float rand1(glm::vec2 point);
glm::vec2 rand2(glm::vec2 point);

float perlin2D(glm::vec2 inPos, float factorA, float factorB, float factorC);
float voroni2D(glm::vec2 inPos, float factorA, float factorB, float factorC);
float vorotwist2D(glm::vec2 inPos, float factorA, float factorB, float factorC);
float voroblur2D(glm::vec2 inPos, float factorA, float factorB, float factorC);
float random2D(glm::vec2 inPos, float factorA, float factorB, float factorC);

float truchetPattern(glm::vec2 pos, int mode, float varA, float varB, float varC);
float halftoneDot2D(glm::vec2 pos, float varA, float varB, float varC);
float cellularA_2D(glm::vec2 pos, float varA, float varB, float varC);
glm::vec3 cellularB_2D(glm::vec2 x, float varA, float varB, float varC);

// Patterns
float lines(float pos, float scale, float brightnessA);
float strips(glm::vec2 pos, float scale, float brightnessA, float cutScale, float fcontrast, float width);
float canvasPockets(glm::vec2 pos, float dotScalar);
float canvas_typeA_2D(glm::vec2 pos, float scale, float varA, float varB, float varC);

#endif