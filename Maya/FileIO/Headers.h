#ifndef HEADERS_MAYA_H
#define HEADERS_MAYA_H

/**
* Each Header will need a method for reading and writing
* Along with reading and writing the internal data of the header
/**/

#include "../MathVector.h"

enum HeaderTypes
{
	Mesh = 0,
	Lights,
	Additional,
	Animations,
	Textures
};

struct Header
{
	Header(HeaderTypes type) : type(type) {}
	HeaderTypes type;
};

struct Vertex
{
	vec3 position;
	vec4 color;
	vec3 normal;
	vec2 uv;
	unsigned int boneID;
	vec3 weights;
	vec3 tangent;
	vec3 bitangent;
};

struct GeometryTransform
{
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

struct MeshData
{
	unsigned int numberOfVerts;
	unsigned int numberOfIndices;
	unsigned int numberOfTransforms;
	Vertex* verts;
	unsigned int* indices;
	GeometryTransform* transforms;
};

struct MeshDataHeader : Header
{
	MeshDataHeader() : Header(Mesh) {}
	unsigned int numberOfMeshes;
	MeshData* meshes;
};


struct Light
{
	vec3 position;
	vec4 color;
	float intensity;
};

struct LightsHeader : Header
{
	LightsHeader() : Header(Mesh) {}
	unsigned int numberOfLights;
	Light* lights;
};

//I'm not sure how to connect textures to mesh yet
struct Texture
{
	const char* textureName;
	unsigned int textureDataLength;
	void* textureData;
	int width;
	int height;
};

struct TextureHeader : Header
{
	TextureHeader() : Header(Mesh) {}
	unsigned int numberOfTextures;
	Texture* textures;
};

struct AdditionalHeader : Header
{
	AdditionalHeader() : Header(Mesh) {}
	unsigned int numberOfAdditonalHeaders;
	Header* headers;
};

struct FileHeader
{
	unsigned int fileType;
	MeshDataHeader* meshes;
	LightsHeader* lights;
	TextureHeader* textures;
	AdditionalHeader* additionalHeaders;
};

#endif /* HEADERS_MAYA_H */