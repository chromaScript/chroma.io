#ifndef MATH_H
#define MATH_H

#include <glm.hpp>
#include <gtc/quaternion.hpp>

#include <vector>

// Math constants
#define ROOT2 1.4142f
#define MATH_PI 3.14159265358979f

inline glm::vec3 lerpDir(glm::vec3 dirA, glm::vec3 dirB, float t)
{
	glm::vec3 out = glm::normalize(glm::vec3(
		dirA.x + (t * (dirB.x - dirA.x)),
		dirA.y + (t * (dirB.y - dirA.y)),
		dirA.z + (t * (dirB.z - dirA.z))
	));
	return out;
}

inline bool compareVec3_byDistance(glm::vec3 pos1, glm::vec3 pos2, float threshold)
{
	float x = powf(pos2.x - pos1.x, 2);
	float y = powf(pos2.y - pos1.y, 2);
	float z = powf(pos2.z - pos1.z, 2);
	float l = sqrtf(x + y + z);
	if (l >= threshold) { return false; }
	return true;
}
inline bool compareVec3_byXYZ(glm::vec3 pos1, glm::vec3 pos2, float threshold)
{
	if (pos2.x - pos1.x >= threshold) { return false; }
	if (pos2.y - pos1.y >= threshold) { return false; }
	if (pos2.z - pos1.z >= threshold) { return false; }
	return true;
}
inline bool compareDir_byAngle(glm::vec3 dir1, glm::vec3 dir2, float threshold)
{
	float dot = (dir1.x * dir2.x) + (dir1.y * dir2.y) + (dir1.z * dir2.z);
	float angle = acosf(dot);
	if (angle >= threshold) { return false; }
	return true;
}
inline float clampf(float val, float min, float max)
{
	if (val <= min) { return min; }
	if (val >= max) { return max; }
	return val;
}
inline int clampi(int val, int min, int max)
{
	if (val <= min) { return min; }
	if (val >= max) { return max; }
	return val;
}

inline float lerpf(float a, float b, float t)
{
	return a + (clampf(t, 0.0f, 1.0f) * (b - a));
}

inline float lerpf_clamp(float a, float b, float t, float min, float max)
{
	return clampf(lerpf(a, b, t), min, max);
}

inline float clampAngle_180(float angle)
{
	if (angle <= -179.9f)
	{
		float out = abs(angle) - 180;
		out -= (floor(out / 360) * 360);
		out = 180 - out;
		return out;
	}
	else if (angle >= 180.0f)
	{
		float out = angle - 180;
		out -= (floor(out / 360) * 360);
		out = -179.9f + out;
		return out;
	}
	else { return angle; }
}
inline float clampAngle_360(float angle)
{
	if (angle < 0.0f)
	{
		float out = 360 - abs(angle);
		while (out < 0.0f)
		{
			out = 360 - abs(out);
		}
		return out;
	}
	else if (angle > 360.0f)
	{
		float out = angle - 360.0f;
		while (out > 360.0f)
		{
			out = angle - 360.0f;
		}
		return out;
	}
	else if (angle >= 359.5f)
	{
		return 0.0f;
	}
	else { return angle; }
}
// Note that these always assume 'i' is always begins with the min/max range
inline int loopDec(int i, int min, int max)
{
	if (i > min) { return i - 1; }
	if (i == min) { return max; }
	return i;
}
inline int loopInc(int i, int min, int max)
{
	if (i < max) { return i + 1; }
	if (i == max) { return min; }
	return i;
}

// Lines are as follows - [x,x1], [y,y1], [z,x2], [w,y2]
inline glm::vec2 lineIntersect2D(glm::vec4 line1, glm::vec4 line2)
{
	float a1 = line1.w - line1.y;
	float b1 = line1.x - line1.z;
	//float c1 = (line1.x * line1.w) - (line1.z * line1.y);
	float c1 = (a1 * line1.x) + (b1 * line1.y);

	float a2 = line2.w - line2.y;
	float b2 = line2.x - line2.z;
	//float c2 = (line2.x * line2.w) - (line2.z * line2.y);
	float c2 = (a2 * line2.x) + (b2 * line2.y);

	float determinant = (a1 * b2) - (a2 * b1);

	if (determinant == 0)
	{
		return glm::vec2(FLT_MAX, FLT_MAX);
	}
	else
	{
		float x = ((b2 * c1) - (b1 * c2)) / determinant;
		float y = ((a1 * c2) - (a2 * c1)) / determinant;
		return glm::vec2(x, y);
	}
	return glm::vec2(
		((a1 * c2) - (b2 * c1)) / ((a1 * b2) - (a2 * b1)),
		((a2 * c1) - (a1 * c2)) / ((a1 * b2) - (a2 * b1)));
}
// Create normalized direction vector from two points
inline glm::vec3 createDirVec2D(glm::vec3 pointA, glm::vec3 pointB, bool flipY)
{
	if (flipY)
	{
		return glm::normalize(glm::vec3(pointA.x - pointB.x, pointB.y - pointA.y, 0.0));
	}
	else
	{
		return glm::normalize(glm::vec3(pointA.x - pointB.x, pointA.y - pointB.y, 0.0));
	}
}
inline glm::vec3 createDirVec2D(float angle)
{
	glm::quat rotator = glm::quat(glm::vec3(0.0f, 0.0f, angle));
	return glm::vec3(rotator * glm::vec3(1.0f, 0.0f, 0.0f));
}

// Distance from point to line given as glm::dvec4 (x1, y1, x2, y2)
inline float distancePointLine2D(glm::vec2 point, glm::vec4 line)
{
	float a = line.y - line.w;
	float b = line.z - line.x;
	float c = (line.x * line.w) - (line.z * line.y);
	float d = abs((a * point.x) + (b * point.y) + c) / sqrt((a * a) + (b * b));
	return d;
}
inline float distancePointLine2D(glm::vec2 point, glm::vec3 direction, glm::vec3 origin)
{
	glm::vec3 lineP2 = origin + (direction * 100.0f);
	glm::vec4 line = glm::vec4(origin.x, origin.y, lineP2.x, lineP2.y);
	return distancePointLine2D(point, line);
}

inline glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec4 line, bool flipY)
{
	float d = distancePointLine2D(point, line);
	glm::vec3 dir = glm::vec3(createDirVec2D(glm::vec3(line.x, line.y, 0.0f), glm::vec3(line.z, line.w, 0.0f), flipY));
	dir = dir * glm::quat(glm::vec3(0.0f, 0.0f, 90.0f));
	return point + (dir * d);
}

inline glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, glm::vec3 dir, bool flipY)
{
	glm::vec3 lineP2 = lineP1 + (dir * 100.0f);
	glm::vec4 line = glm::vec4(lineP1.x, lineP1.y, lineP2.x, lineP2.y);
	float d = (float)distancePointLine2D(point, line);
	glm::vec3 direction = createDirVec2D(glm::vec3(line.x, line.y, 0.0), glm::vec3(line.z, line.w, 0.0f), flipY);
	direction = direction * glm::quat(glm::vec3(0.0f, 0.0f, 90.0f));
	return point + (direction * d);
}
inline glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, float angle, bool flipY)
{
	return projectPointToLine2D(point, lineP1, createDirVec2D(angle), flipY);
}

#endif