#ifndef MAYA_FILE_EXPORTER_H
#define MAYA_FILE_EXPORTER_H

#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MFnSet.h>
#include <maya/MItDag.h>
#include <maya/MFnMesh.h>

#include "FileIO\BinaryFileWriter.h"
#include <fstream>

//Used to store UV set information
//
struct UVSet {
	MFloatArray	uArray;
	MFloatArray	vArray;
	MString		name;
	UVSet*		next;
};

class MayaFileExporter
{
	std::ofstream& file;
	BinaryFileWriter writer;
	FileHeader fileHeader;

public:
	MayaFileExporter(std::ofstream& stream) : file(stream) {}

	MStatus parseScene(MItDag& dag);
	MStatus writeFile();

private:
	MeshData* getMeshData(MDagPath& path, MStatus& status);
	Light* getLights(MDagPath& path, MStatus& status);
	Texture* getTexture(MDagPath& path, MStatus& status);
	GeometryTransform* getTransform(MDagPath& path, MStatus& status);
	Vertex* getVertex(MFnMesh* mesh, MStatus& status);
	vec2* getUVs(MFnMesh* mesh, MStatus& status, MString currentUVSetName);
};


#endif /*MAYA_FILE_EXPORTER_H*/