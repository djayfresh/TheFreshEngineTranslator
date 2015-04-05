#ifndef MATH_MAYA_H
#define MATH_MAYA_H
#include "ExportHeader.h"
#include <iostream>

struct DLL_SHARED vec3
{
	float x;
	float y;
	float z;
	vec3(): x(0), y(0), z(0) {}
	vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3(float d[3]) : x(d[0]), y(d[1]), z(d[2]) {}
};

inline std::ostream& operator<<(std::ostream& ost, const vec3& vec)
{
	ost << vec.x << vec.y << vec.z;
	return ost;
}

struct DLL_SHARED vec4
{
	float x;
	float y;
	float z;
	float w;
	vec4(): x(0), y(0), z(0), w(0) {}
	vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	vec4(float d[4]) : x(d[0]), y(d[1]), z(d[2]), w(d[3]) {}

};

inline std::ostream& operator<<(std::ostream& ost, const vec4& vec)
{
	ost << vec.x << vec.y << vec.z << vec.w;
	return ost;
}

struct DLL_SHARED vec2
{
	float x;
	float y;
	vec2(): x(0), y(0) {}
	vec2(float x, float y) : x(x), y(y) {}
	vec2(float d[2]) : x(d[0]), y(d[1]) {}

};

inline std::ostream& operator<<(std::ostream& ost, const vec2& vec)
{
	ost << vec.x << vec.y;
	return ost;
}

#endif /* MATH_MAYA_H */