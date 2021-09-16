#ifndef MATH_H
#define MATH_H

#include <glm.hpp>
#include <gtc/quaternion.hpp>

#include <vector>

// Math constants
#define ROOT2 1.4142f
#define MATH_PI 3.14159265358979f
#define MATH_RAD_90 90.0f * (MATH_PI / 180.0f)
#define MATH_RAD_180 180.0f * (MATH_PI / 180.0f)

glm::vec3 lerpDir(glm::vec3 dirA, glm::vec3 dirB, float t);
bool compareVec3_byDistance(glm::vec3 pos1, glm::vec3 pos2, float threshold);
bool compareVec3_byXYZ(glm::vec3 pos1, glm::vec3 pos2, float threshold);
bool compareDir_byAngle(glm::vec3 dir1, glm::vec3 dir2, float threshold);
float clampf(float val, float min, float max);
int clampi(int val, int min, int max);
double clampd(double val, double min, double max);

float lerpf(float a, float b, float t);

double lerpd(double a, double b, double t);

float lerpf_clamp(float a, float b, float t, float min, float max);

float clampAngle_180(float angle);
float clampAngle_360(float angle);
float percentRange(float value, float min, float max, bool rawInput);
float percentRange_squared(float value, float min, float max, bool rawInput);
float percentRange_squaredInvert(float value, float min, float max, bool rawInput);
float percentRange_squaredCenter(float value, float min, float max, bool rawInput);
float percentRange_cubed(float value, float min, float max, bool rawInput);
float percentRange_cubedInvert(float value, float min, float max, bool rawInput);
float percentRange_cubedCenter(float value, float min, float max, bool rawInput);
int loopDec(int i, int min, int max);
int loopInc(int i, int min, int max);
glm::vec2 lineIntersect2D(glm::vec4 line1, glm::vec4 line2);
glm::vec3 createDirVec2D(glm::vec3 pointA, glm::vec3 pointB, bool flipY);
glm::vec3 createDirVec2D(float angle);
float distancePointLine2D(glm::vec2 point, glm::vec4 line);
float distancePointLine2D(glm::vec2 point, glm::vec3 direction, glm::vec3 origin);

glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec4 line, bool flipY);

glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, glm::vec3 dir, bool flipY);
glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, float angle, bool flipY);

#endif