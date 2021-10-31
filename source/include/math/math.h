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
#define DEFAULT_DIR glm::vec3(1.0f, 0.0f, 0.0f)

glm::vec3 makeDir(glm::vec3 pos1, glm::vec3 pos2);

glm::vec3 lerpDir(glm::vec3 dirA, glm::vec3 dirB, float t);
glm::vec3 lerpPos(glm::vec3 posA, glm::vec3 posB, float t);
bool compareVec3_byDistance(glm::vec3 pos1, glm::vec3 pos2, float threshold);
bool compareVec3_isEqual(glm::vec3* first, glm::vec3* second, float threshold);
bool compareVec3_byXYZ(glm::vec3 pos1, glm::vec3 pos2, float threshold);
bool compareDir_byAngle(glm::vec3 dir1, glm::vec3 dir2, float threshold);
float rad_fromDir2(glm::vec3* dir);
float rad_fromDir2(glm::vec2* dir);

float clampf(float val, float min, float max);
int clampi(int val, int min, int max);
double clampd(double val, double min, double max);
int lerpi(int a, int b, float t);
float lerpf(float a, float b, float t);
double lerpd(double a, double b, double t);
float lerpf_clamp(float a, float b, float t, float min, float max);

float smoothstep(float edge0, float edge1, float input);
float step(float edge, float x);

glm::vec3 mix(glm::vec3 x, glm::vec3 y, float a);
glm::vec2 mix(glm::vec2 x, glm::vec2 y, float a);
float mix(float x, float y, float a);

glm::vec2 floor2(glm::vec2 point);
glm::vec3 floor3(glm::vec3 point);
glm::vec4 floor4(glm::vec4 point);

float fract(float value);
glm::vec2 fract(glm::vec2 point);
glm::vec3 fract(glm::vec3 coord);
glm::vec4 fract(glm::vec4 normal);

float sumVector_float(std::vector<float>* input);
float clampAngle_180(float angle);
float clampAngle_360(float angle);
double clampAngle_180d(double angle);
double clampAngle_360d(double angle);
float clampRange(float value);
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
float distancePointLineSegment(glm::vec2 point, glm::vec2 start, glm::vec2 end);

glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec4 line, bool flipY);

glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, glm::vec3 dir, bool flipY);
glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, float angle, bool flipY);

float lengthPointVec(std::vector<glm::vec3>* points);
float lengthPointVec(std::vector<std::pair<float, glm::vec3>>* points);

#endif