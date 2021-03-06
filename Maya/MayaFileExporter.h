#ifndef MAYA_FILE_EXPORTER_H
#define MAYA_FILE_EXPORTER_H

#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MFnSet.h>
#include <maya/MItDag.h>
#include <maya/MFnMesh.h>


#include "ExportHeader.h"
#include "FileIO\BinaryFileWriter.h"
#include <fstream>
#include <vector>
#include <map>

//Used to store UV set information
//
struct UVSet {
	MFloatArray	uArray;
	MFloatArray	vArray;
	MString		name;
	UVSet*		next;
};

//Map comparitor
struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return std::strcmp(a, b) < 0;
   }
};


class MayaFileExporter
{
	std::ofstream& file;
	FileHeader fileHeader;

public:
	MayaFileExporter(std::ofstream& stream) : file(stream) {}

	MStatus parseScene();
	MStatus parseSelection();
	MStatus writeFile();
private:
	/*Scene Data Collection*/
	MeshData* getMeshData(MDagPath& path, MStatus& status);
	Light* getLights(MDagPath& path, MStatus& status);
	Texture* getTexture(MDagPath& path, MStatus& status);
	GeometryTransform* getTransform(MDagPath& path, MStatus& status);
	Vertex* getVertex(MFnMesh* mesh, MStatus& status, uint& numberOfVerts);
	vec2* getUVs(MFnMesh* mesh, MStatus& status, MString currentUVSetName, uint& numberOfUVs);
	uint* getIndices(MFnMesh* mesh, MStatus& status, uint& numbeOfIndices);

	/*Parsing Scene*/
	MStatus parseMeshes();
	void parseMeshPath(MDagPath& dagPath, MStatus& status);
	MStatus parseLights();
	void parseLightPath(MDagPath& dagpath, MStatus& status);

	/*Headers*/
	MeshDataHeader* createMeshHeader();
	AdditionalHeader* createAdditionalHeaders();
	LightsHeader* createLightsHeader();
	TextureHeader* createTextureHeader();

	/*Cleanup*/
	void cleanup(FileHeader& header);
	MStatus cleanUp();
private:
	
	std::map<const char*, MeshData*, cmp_str> meshes;
	std::vector<Light*> lights;
	std::vector<Texture*> textures;
};


#endif /*MAYA_FILE_EXPORTER_H*/