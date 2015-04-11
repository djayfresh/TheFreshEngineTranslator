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
	Header(HeaderTypes type) : type((int)type) {}
	int type;
};

struct Vertex
{
	vec3 position;
	vec4 color;
	vec3 normal;
	vec2 uv;
	vec3 tangent;
	vec3 bitangent;
};

struct GeometryTransform
{
	unsigned int nameLength;
	vec3 position;
	vec3 rotation;
	vec3 scale;
	const char* geometryName;
	
	static const unsigned int Name_Length_Offset = 0;
	static const unsigned int Position_Offset = 4; //Set to zero, ignoring name
	static const unsigned int Rotation_Offset = 16;
	static const unsigned int Scale_Offset = 28;
	static const unsigned int Name_Pointer_Offset = 32;
};

struct MeshData
{
	unsigned int numberOfVerts;
	unsigned int numberOfIndices;
	unsigned int numberOfTransforms;
	unsigned int nameLength;
	Vertex* verts;
	unsigned int* indices;
	GeometryTransform* transforms;
	const char* shapeName;
	/*Name will be written here*/
	//const char* name
	
	static const unsigned int Num_Of_Verts_Offset = 0; //Ignore Name information
	static const unsigned int Num_Of_Indices_Offset = 4;
	static const unsigned int Num_Of_Transforms_Offset = 8;
	static const unsigned int Name_Length_Offset = 12;
	static const unsigned int Vertex_Offset = 16;
	static const unsigned int Indices_Offset = 20;
	static const unsigned int Transforms_Offset = 24;
	static const unsigned int Name_Offset = 28;
};

struct MeshDataHeader : Header
{
	MeshDataHeader() : Header(Mesh), numberOfMeshes(0), meshes(NULL) {}
	unsigned int numberOfMeshes;
	MeshData* meshes;

	static const unsigned int Num_Of_Meshes_Offset = 4;
	static const unsigned int Mesh_Data_Offset = 8;
};


struct Light
{
	vec3 position;
	vec4 color;
	vec4 intensity;
	Light() : position(vec3()), color(vec4()), intensity(vec4()) {}
	Light(vec3 position, vec4 color, vec4 intensity) : position(position), color(color), intensity(intensity) {}
};

struct LightsHeader : Header
{
	LightsHeader() : Header(Lights), numberOfLights(0), lights(NULL) {}
	unsigned int numberOfLights;
	Light* lights;
};

//I'm not sure how to connect textures to mesh yet
struct Texture
{
	unsigned int nameLength;
	unsigned int textureDataLength;
	int width;
	int height;
	void* textureData;
	const char* textureName;
};

struct TextureHeader : Header
{
	TextureHeader() : Header(Textures), numberOfTextures(0), textures(NULL) {}
	unsigned int numberOfTextures;
	Texture* textures;
};

struct AdditionalHeader : Header
{
	AdditionalHeader() : Header(Additional), numberOfAdditonalHeaders(0), headers(NULL) {}
	unsigned int numberOfAdditonalHeaders;
	Header** headers;
};

struct FileHeader
{
	unsigned int fileType;
	MeshDataHeader* meshes;
	LightsHeader* lights;
	TextureHeader* textures;
	AdditionalHeader* additionalHeaders;

	FileHeader() : fileType(0), meshes(NULL), lights(NULL), textures(NULL), additionalHeaders(NULL) {}
	FileHeader(unsigned int fileType, MeshDataHeader* mesh, LightsHeader* light, TextureHeader* texture, AdditionalHeader* additionalHeaders) : fileType(fileType), meshes(mesh), lights(light), textures(texture), additionalHeaders(additionalHeaders) {}

	static const unsigned int MeshHeaderOffset = 4;
	static const unsigned int LightHeaderOffset = 8;
	static const unsigned int TextureHeaderOffset = 12;
	static const unsigned int AdditionalHeaderOffset = 16;
};

#endif /* HEADERS_MAYA_H */