#ifndef MATH_MAYA_H
#define MATH_MAYA_H
#include <iostream>

struct vec3
{
	float x;
	float y;
	float z;
};

std::ostream& operator<<(std::ostream& ost, const vec3& vec)
{
	ost << vec.x << vec.y << vec.z;
	return ost;
}

struct vec4
{
	float x;
	float y;
	float z;
	float w;
};

std::ostream& operator<<(std::ostream& ost, const vec4& vec)
{
	ost << vec.x << vec.y << vec.z << vec.w;
	return ost;
}

struct vec2
{
	float x;
	float y;
};

std::ostream& operator<<(std::ostream& ost, const vec2& vec)
{
	ost << vec.x << vec.y;
	return ost;
}

#endif /* MATH_MAYA_H */