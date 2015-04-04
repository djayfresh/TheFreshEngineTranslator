#ifndef HANDLER_FILE_EXPORT_H
#define HANDLER_FILE_EXPORT_H

#include "../FileIO/Headers.h"
#include "../MathVector.h"

class FileExport
{
public:
	MeshData createData(const char* shapeName, unsigned int numberOfVerts, unsigned int numberOfIndices, unsigned int numberOfTransforms, Vertex* verts, unsigned int* indices, GeometryTransform* transforms);
	GeometryTransform createTransform(const char* transformName, vec3 postion, vec3 rotation, vec3 scale);
};

#endif /* HANDLER_FILE_EXPORT_H */