#include "../include/math/math.h"

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <vector>
#include <cmath>

// Math constants
#define ROOT2 1.4142f
#define MATH_PI 3.14159265358979f
#define MATH_RAD_90 90.0f * (MATH_PI / 180.0f)
#define MATH_RAD_180 180.0f * (MATH_PI / 180.0f)

// Create normalized 2D direction vector from two positions. Direction flows from pos1 to pos2.
glm::vec3 makeDir(glm::vec3 pos1, glm::vec3 pos2)
{
	return glm::normalize(glm::vec3(
		pos2.x - pos1.x,
		pos2.y - pos1.y,
		0.0f));
}
float makeAngle(glm::vec3 pos1, glm::vec3 pos2)
{
	glm::vec3 dir = makeDir(pos1, pos2);
	return glm::degrees(atan2f(dir.y, dir.x));
}

// Compare two angles from range -180 to 180
bool compareAngle(float angleA, float angleB, float threshold)
{
	float dif = abs((angleA + 180.0f) - (angleB + 180.0f));
	if (dif > 180.0f) { dif = 360.0f - dif; }
	if (dif >= threshold) { return true; }
	return false;
}
bool compareParallel(float angleA, float angleB, float threshold)
{
	float dif = abs((angleA + 180.0f) - (angleB + 180.0f));
	if (dif > 180.0f) { dif = 360.0f - dif; }
	if (abs(180.0f - dif) <= threshold) { return true; }
	return false;
}
bool comparePerpendicular(float angleA, float angleB, float threshold)
{
	float dif = abs((angleA + 180.0f) - (angleB + 180.0f));
	if (dif > 180.0f) { dif = 360.0f - dif; }
	if (abs(90.0f - dif) <= threshold) { return true; }
	return false;
}

// Return the difference between two angles in +/-180 range
float angleDifference(float angleA, float angleB)
{
	float dif = abs((angleA + 180.0f) - (angleB + 180.0f));
	if (dif > 180.0f) { dif = 360.0f - dif; }
	return dif;
}

glm::vec3 lerpDir(glm::vec3 dirA, glm::vec3 dirB, float t)
{
	glm::vec3 out = glm::normalize(glm::vec3(
		dirA.x + (t * (dirB.x - dirA.x)),
		dirA.y + (t * (dirB.y - dirA.y)),
		dirA.z + (t * (dirB.z - dirA.z))
	));
	return out;
}

glm::vec3 lerpPos(glm::vec3 posA, glm::vec3 posB, float t)
{
	glm::vec3 out = glm::vec3(
		posA.x + (t * (posB.x - posA.x)),
		posA.y + (t * (posB.y - posA.y)),
		posA.z + (t * (posB.z - posA.z))
	);
	return out;
}

bool compareVec3_byDistance(glm::vec3 pos1, glm::vec3 pos2, float threshold)
{
	float x = powf(pos2.x - pos1.x, 2);
	float y = powf(pos2.y - pos1.y, 2);
	float z = powf(pos2.z - pos1.z, 2);
	float l = sqrtf(x + y + z);
	if (l >= threshold) { return false; }
	return true;
}
bool compareVec3_isEqual(glm::vec3* first, glm::vec3* second, float threshold)
{ 
	if (abs(first->x - second->x) >= threshold) { return false; }
	if (abs(first->y - second->y) >= threshold) { return false; }
	if (abs(first->z - second->z) >= threshold) { return false; }
	return true;
}
bool compareVec3_byXYZ(glm::vec3 pos1, glm::vec3 pos2, float threshold)
{
	if (pos2.x - pos1.x >= threshold) { return false; }
	if (pos2.y - pos1.y >= threshold) { return false; }
	if (pos2.z - pos1.z >= threshold) { return false; }
	return true;
}
bool compareDir_byAngle(glm::vec3 dir1, glm::vec3 dir2, float threshold)
{
	float dot = (dir1.x * dir2.x) + (dir1.y * dir2.y) + (dir1.z * dir2.z);
	float angle = acosf(dot);
	if (angle >= threshold) { return false; }
	return true;
}

float rad_fromDir2(glm::vec3* dir)
{
	return std::atan2f(dir->x, dir->y);
}
float rad_fromDir2(glm::vec2* dir)
{
	return std::atan2f(dir->x, dir->y);
}

float clampf(float val, float min, float max)
{
	if (val <= min) { return min; }
	if (val >= max) { return max; }
	return val;
}
int clampi(int val, int min, int max)
{
	if (val <= min) { return min; }
	if (val >= max) { return max; }
	return val;
}
double clampd(double val, double min, double max)
{
	if (val <= min) { return min; }
	if (val >= max) { return max; }
	return val;
}

int lerpi(int a, int b, float t)
{
	return int(a + (clampf(t, 0.0f, 1.0f) * (b - a)));
}

float lerpf(float a, float b, float t)
{
	return a + (clampf(t, 0.0f, 1.0f) * (b - a));
}

double lerpd(double a, double b, double t)
{
	return a + (clampd(t, 0.0, 1.0) * (b - a));
}

float lerpf_clamp(float a, float b, float t, float min, float max)
{
	return clampf(lerpf(a, b, t), min, max);
}

float clampAngle_180(float angle)
{
	if (angle <= -179.9f)
	{
		float out = abs(angle) - 180.0f;
		out -= (floor(out / 360.0f) * 360.0f);
		out = 180.0f - out;
		return out;
	}
	else if (angle >= 180.0f)
	{
		float out = angle - 180.0f;
		out -= (floor(out / 360.0f) * 360.0f);
		out = -179.9f + out;
		return out;
	}
	else { return angle; }
}
float clampAngle_360(float angle)
{
	if (angle < 0.0f)
	{
		float out = 360.0f - abs(angle);
		while (out < 0.0f)
		{
			out = 360.0f - abs(out);
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
double clampAngle_180d(double angle)
{
	if (angle <= -179.9)
	{
		float out = abs(angle) - 180.0;
		out -= (floor(out / 360.0) * 360.0);
		out = 180 - out;
		return out;
	}
	else if (angle >= 180.0)
	{
		float out = angle - 180.0;
		out -= (floor(out / 360.0) * 360.0);
		out = -179.9 + out;
		return out;
	}
	else { return angle; }
}
double clampAngle_360d(double angle)
{
	if (angle < 0.0)
	{
		float out = 360.0 - abs(angle);
		while (out < 0.0)
		{
			out = 360.0 - abs(out);
		}
		return out;
	}
	else if (angle > 360.0)
	{
		float out = angle - 360.0;
		while (out > 360.0)
		{
			out = angle - 360.0;
		}
		return out;
	}
	else if (angle >= 359.5)
	{
		return 0.0;
	}
	else { return angle; }
}

// LoopClamp input value between 0 to 1
float clampRange(float value)
{
	if (value < 0.0f)
	{
		return 1.0f - fract(value);
	}
	else if (value > 1.0f)
	{
		return 0.0f + fract(value);;
	}
	else { return value; }
}

float sumVector_float(std::vector<float>* input)
{
	if (input->size() == 0) { return 0.0f; }
	float sum = 0.0f;
	for (float f : *input)
	{
		sum += f;
	}
	return sum;
}

float smoothstep(float edge0, float edge1, float input)
{
	float t = clampf((input - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return t * t * (3.0f - 2.0f * t);
}
float step(float edge, float x)
{
	if (x < edge) { return 0.0f; }
	else { return 1.0f; }
}

glm::vec3 mix(glm::vec3 x, glm::vec3 y, float a)
{
	return x * (1.0f - a) + y * a;
}
glm::vec2 mix(glm::vec2 x, glm::vec2 y, float a)
{
	return x * (1.0f - a) + y * a;
}
float mix(float x, float y, float a)
{
	return x * (1.0f - a) + y * a;
}

glm::vec2 floor2(glm::vec2 point)
{
	return glm::vec2(std::floorf(point.x), std::floorf(point.y));
}
glm::vec3 floor3(glm::vec3 point)
{
	return glm::vec3(std::floorf(point.x), std::floorf(point.y), std::floorf(point.z));
}
glm::vec4 floor4(glm::vec4 point)
{
	return glm::vec4(std::floorf(point.x), std::floorf(point.y), std::floorf(point.z), std::floorf(point.w));
}

float fract(float value)
{
	return value - std::floorf(value);
}
glm::vec2 fract(glm::vec2 point)
{
	return glm::vec2(point.x - std::floorf(point.x), point.y - std::floorf(point.y));
}
glm::vec3 fract(glm::vec3 coord)
{
	return glm::vec3(coord.x - std::floorf(coord.x), coord.y - std::floorf(coord.y), coord.z - std::floorf(coord.z));
}
glm::vec4 fract(glm::vec4 normal)
{
	return glm::vec4(normal.x - std::floorf(normal.x), normal.y - std::floorf(normal.y), 
		normal.z - std::floorf(normal.z), normal.w - std::floorf(normal.w));
}

// Linear interp between range values
float percentRange(float value, float min, float max, bool rawInput)
{
	if (max - min == 0.0f) { return 1.0f; } // Return 1.0f to prevent divide by 0
	// Handle when the input is to be returned as a percentage
	if (rawInput)
	{
		return clampf((value - min) / (max - min), 0.0f, 1.0f);
	}
	// Handle when the input is to be returned as a raw value within the range
	else
	{
		return (clampf(value, 0.0f, 1.0f) * (max - min)) + min;
	}
}

// Non-linear interp between range values
float percentRange_squared(float value, float min, float max, bool rawInput)
{
	if (max - min == 0.0f) { return 1.0f; } // Return 1.0f to prevent divide by 0
	// Handle when the input is to be returned as a percentage
	if (rawInput)
	{
		return sqrtf(clampf((value - min) / (max - min), 0.0f, 1.0f));
	}
	// Handle when the input is to be returned as a raw value within the range
	else
	{
		return (powf(clampf(value, 0.0f, 1.0f), 2) * (max - min)) + min;
	}
}
// Non-linear interp between range values
float percentRange_squaredInvert(float value, float min, float max, bool rawInput)
{
	if (max - min == 0.0f) { return 1.0f; } // Return 1.0f to prevent divide by 0
	// Handle when the input is to be returned as a percentage
	if (rawInput)
	{
		return lerpf(1.0f, 0.0f, sqrtf(1 - clampf((value - min) / (max - min), 0.0f, 1.0f)));
	}
	// Handle when the input is to be returned as a raw value within the range
	else
	{
		return (lerpf(1.0f, 0.0f, powf(1 - clampf(value, 0.0f, 1.0f), 2)) * (max - min)) + min;
	}
}
// Non-linear interp between range values for centered values (Where the neutral position is halfwaay in the range)
float percentRange_squaredCenter(float value, float min, float max, bool rawInput)
{
	if (max - min == 0.0f) { return 1.0f; } // Return 1.0f to prevent divide by 0
	// Handle when the input is to be returned as a percentage
	if (rawInput)
	{
		float percent = clampf((value - min) / (max - min), 0.0f, 1.0f);
		if (percent == 0.500f) { return 0.5f; }
		else if (percent > 0.500f)
		{
			float val = lerpf_clamp(0.0f, 1.0f, (percent * 2.0f) - 1.0f, 0.0f, 1.0f);
			//return ((powf(val, 1.0f / 3.0f) / 2.0f) + 0.5f);
			return lerpf(0.5f, 1.0f, powf(val, 1.0f / 2.0f));
		}
		else
		{
			float val = lerpf_clamp(1.0f, 0.0f, percent * 2.0f, 0.0f, 1.0f);
			//return ((powf(val, 1.0f / 3.0f) * 2.0f));
			return lerpf(0.5f, 0.0f, powf(val, 1.0f / 2.0f));
		}
	}
	// Handle when the input is to be returned as a raw value within the range
	// Note that this comes in as % of a slider along a given length
	else
	{
		if (value == 0.500f) { return (0.5f * (max - min)) + min; }
		else if (value > 0.500f)
		{
			float val = lerpf_clamp(0.0f, 1.0f, (value * 2.0f) - 1.0f, 0.0f, 1.0f);
			return (((powf(val, 2) / 2.0f) + 0.5f) * (max - min)) + min;
		}
		else
		{
			float val = lerpf_clamp(1.0f, 0.0f, value * 2.0f, 0.0f, 1.0f);
			return -(min + (((powf(val, 2) / 2.0f) + 0.5f) * (max - min)));
		}
	}
}

// Non-linear interp between range values
float percentRange_cubed(float value, float min, float max, bool rawInput)
{
	if (max - min == 0.0f) { return 1.0f; } // Return 1.0f to prevent divide by 0
	// Handle when the input is to be returned as a percentage
	if (rawInput)
	{
		return cbrtf(clampf((value - min) / (max - min), 0.0f, 1.0f));
	}
	// Handle when the input is to be returned as a raw value within the range
	else
	{
		return (powf(clampf(value, 0.0f, 1.0f), 3) * (max - min)) + min;
	}
}
// Non-linear interp between range values
float percentRange_cubedInvert(float value, float min, float max, bool rawInput)
{
	if (max - min == 0.0f) { return 1.0f; } // Return 1.0f to prevent divide by 0
	// Handle when the input is to be returned as a percentage
	if (rawInput)
	{
		return lerpf(1.0f, 0.0f, cbrtf(1 - clampf((value - min) / (max - min), 0.0f, 1.0f)));
	}
	// Handle when the input is to be returned as a raw value within the range
	else
	{
		return (lerpf(1.0f, 0.0f, powf(1 - clampf(value, 0.0f, 1.0f), 3)) * (max - min)) + min;
	}
}

// Non-linear interp between range values for centered values (Where the neutral position is halfwaay in the range)
float percentRange_cubedCenter(float value, float min, float max, bool rawInput)
{
	if (max - min == 0.0f) { return 1.0f; } // Return 1.0f to prevent divide by 0
	// Handle when the input is to be returned as a percentage
	if (rawInput)
	{
		float percent = clampf((value - min) / (max - min), 0.0f, 1.0f);
		if (percent == 0.500f) { return 0.5f; }
		else if (percent > 0.500f)
		{
			float val = lerpf_clamp(0.0f, 1.0f, (percent * 2.0f) - 1.0f, 0.0f, 1.0f);
			//return ((powf(val, 1.0f / 3.0f) / 2.0f) + 0.5f);
			return lerpf(0.5f, 1.0f, powf(val, 1.0f / 3.0f));
		}
		else
		{
			float val = lerpf_clamp(1.0f, 0.0f, percent * 2.0f, 0.0f, 1.0f);
			//return ((powf(val, 1.0f / 3.0f) * 2.0f));
			return lerpf(0.5f, 0.0f, powf(val, 1.0f / 3.0f));
		}
	}
	// Handle when the input is to be returned as a raw value within the range
	// Note that this comes in as % of a slider along a given length
	else
	{
		if (value == 0.500f) { return (0.5f * (max - min)) + min; }
		else if (value > 0.500f)
		{
			float val = lerpf_clamp(0.0f, 1.0f, (value * 2.0f) - 1.0f, 0.0f, 1.0f);
			return (((powf(val, 3) / 2.0f) + 0.5f) * (max - min)) + min;
		}
		else
		{
			float val = lerpf_clamp(1.0f, 0.0f, value * 2.0f, 0.0f, 1.0f);
			return -(min + (((powf(val, 3) / 2.0f) + 0.5f) * (max - min)));
		}
	}
}

// Note that these always assume 'i' is always begins with the min/max range
int loopDec(int i, int min, int max)
{
	if (i > min) { return i - 1; }
	if (i == min) { return max; }
	return i;
}
int loopInc(int i, int min, int max)
{
	if (i < max) { return i + 1; }
	if (i == max) { return min; }
	return i;
}

// Lines are as follows - [x,x1], [y,y1], [z,x2], [w,y2]
glm::vec2 lineIntersect2D(glm::vec4 line1, glm::vec4 line2)
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
glm::vec3 lineIntersect2D(glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2) {
	return glm::vec3(lineIntersect2D(glm::vec4(p1.x, p1.y, q1.x, q1.y), glm::vec4(p2.x, p2.y, q2.x, q2.y)), 0.0f);
}
// Create normalized direction vector from two points
glm::vec3 createDirVec2D(glm::vec3 pointA, glm::vec3 pointB, bool flipY)
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
glm::vec3 createDirVec2D(float angle)
{
	glm::quat rotator = glm::quat(glm::vec3(0.0f, 0.0f, angle));
	return glm::vec3(rotator * glm::vec3(1.0f, 0.0f, 0.0f));
}

// Distance from point to line given as glm::dvec4 (x1, y1, x2, y2)
float distancePointLine2D(glm::vec2 point, glm::vec4 line)
{
	float a = line.y - line.w;
	float b = line.z - line.x;
	float c = (line.x * line.w) - (line.z * line.y);
	float d = abs((a * point.x) + (b * point.y) + c) / sqrt((a * a) + (b * b));
	return d;
}
float distancePointLine2D(glm::vec3 point, glm::vec4 line)
{
	float a = line.y - line.w;
	float b = line.z - line.x;
	float c = (line.x * line.w) - (line.z * line.y);
	float d = abs((a * point.x) + (b * point.y) + c) / sqrt((a * a) + (b * b));
	return d;
}
float distancePointLine2D(glm::vec3 point, glm::vec3 q1, glm::vec3 p1)
{
	return distancePointLine2D(point, glm::vec4(q1.x, q1.y, p1.x, p1.y));
}
float distancePointLine2D(glm::vec2 point, glm::vec3 direction, glm::vec3 origin)
{
	glm::vec3 lineP2 = origin + (direction * 100.0f);
	glm::vec4 line = glm::vec4(origin.x, origin.y, lineP2.x, lineP2.y);
	return distancePointLine2D(point, line);
}

float distancePointLineSegment(glm::vec3 point, glm::vec3 start, glm::vec3 end) 
{
	return distancePointLineSegment(glm::vec2(point), glm::vec2(start), glm::vec2(end));
}
float distancePointLineSegment(glm::vec2 point, glm::vec2 start, glm::vec2 end) 
{
	float A = point.x - start.x;
	float B = point.y - start.y;
	float C = end.x - start.x;
	float D = end.y - start.y;

	float dot1 = A * C + B * D;
	float len_sq = C * C + D * D;
	float param = -1.0;
	if (len_sq != 0.0) { param = dot1 / len_sq; }

	float xx, yy;

	if (param < 0.0) {
		xx = start.x;
		yy = start.y;
	}

	else if (param > 1.0) {
		xx = end.x;
		yy = end.y;
	}

	else {
		xx = start.x + param * C;
		yy = start.y + param * D;
	}

	float dx = point.x - xx;
	float dy = point.y - yy;
	return sqrt(dx * dx + dy * dy);
}

glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec4 line, bool flipY)
{
	float d = distancePointLine2D(point, line);
	glm::vec3 dir = glm::vec3(createDirVec2D(glm::vec3(line.x, line.y, 0.0f), glm::vec3(line.z, line.w, 0.0f), flipY));
	dir = dir * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	return point + (dir * d);
}

glm::vec3 projectPointToLine2D(glm::vec3 point, std::pair<glm::vec3, glm::vec3> line, bool flipY)
{
	return projectPointToLine2D(point, glm::vec4(line.first.x, line.first.y, line.second.x, line.second.y), flipY);
}

glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, glm::vec3 dir, bool flipY)
{
	glm::vec3 lineP2 = lineP1 + (dir * 100.0f);
	glm::vec4 line = glm::vec4(lineP1.x, lineP1.y, lineP2.x, lineP2.y);
	float d = (float)distancePointLine2D(point, line);
	glm::vec3 direction = createDirVec2D(glm::vec3(line.x, line.y, 0.0), glm::vec3(line.z, line.w, 0.0f), flipY);
	direction = direction * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / 2.0f));
	return point + (direction * d);
}
glm::vec3 projectPointToLine2D(glm::vec3 point, glm::vec3 lineP1, float angle, bool flipY)
{
	return projectPointToLine2D(point, lineP1, createDirVec2D(angle), flipY);
}

float lengthPointVec(std::vector<glm::vec3>* points)
{
	if (points->size() < 2) { return 0.0f; }
	float outLen = 0.0f;
	for (size_t i = 1; i < points->size(); i++) {
		outLen += glm::length(points->at(i) - points->at(i - 1));
	}
	return outLen;
}

float lengthPointVec(std::vector<std::pair<float, glm::vec3>>* points)
{
	if (points->size() < 2) { return 0.0f; }
	float outLen = 0.0f;
	for (size_t i = 1; i < points->size(); i++) {
		outLen += glm::length(points->at(i).second - points->at(i - 1).second);
	}
	return outLen;
}

float pointLineDeterminant(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 point)
{
	return ((pos2.x - pos1.x) * (point.y - pos1.y)) - ((pos2.y - pos1.y) * (point.x - pos1.x));
}

bool lineSegCCW(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	return (p3.y - p1.y) * (p2.x - p1.x) > (p2.y - p1.y) * (p3.x - p1.x);
}
bool lineSegmentIntersection(glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2)
{
	return lineSegCCW(p1, p2, q2) != lineSegCCW(q1, p2, q2) && lineSegCCW(p1, q1, p2) != lineSegCCW(p1, q1, q2);
}