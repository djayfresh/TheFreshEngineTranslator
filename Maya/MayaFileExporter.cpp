#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MFnMesh.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MFnSet.h>

#include "FileIO\BinaryFileWriter.h"
#include <fstream>
#include "ExportHeader.h"

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

MeshData* MayaFileExporter::getMeshData(MDagPath& path, MStatus& status)
{
	MDagPath* fDagPath = new MDagPath(path);
	MFnMesh* fMesh = new MFnMesh(*fDagPath, &status);

    if (!status && status.statusCode () == MStatus::kInvalidParameter)
        return NULL;

	MeshData* mesh = new MeshData();



	return mesh;
}

GeometryTransform* MayaFileExporter::getTransform(MDagPath& path, MStatus& status)
{

	GeometryTransform* transform = new GeometryTransform();

	MMatrix transformNode = path.inclusiveMatrix(&status);
    // This node has no transform - i.e., it’s the world node
    if (!status && status.statusCode () == MStatus::kInvalidParameter)
        return NULL;

	MTransformationMatrix matrix (transformNode); 
	MVector translation = matrix.translation(MSpace::kWorld);
	transform->position = vec3((float)translation.x, (float)translation.y, (float)translation.z);

    double threeDoubles[3];
    MTransformationMatrix::RotationOrder rOrder;

    matrix.getRotation (threeDoubles, rOrder, MSpace::kWorld);
	transform->rotation = vec3((float)threeDoubles[0],(float)threeDoubles[1],(float)threeDoubles[2]);

    matrix.getScale (threeDoubles, MSpace::kWorld);
	transform->scale = vec3((float)threeDoubles[0],(float)threeDoubles[1],(float)threeDoubles[2]);

	MFnDagNode dagNode(path, &status);
	if (!status && status.statusCode () == MStatus::kInvalidParameter)
        return NULL;

	transform->geometryName = dagNode.name().asChar();
	transform->nameLength = stringLength(transform->geometryName);

	return transform;
}

Vertex* MayaFileExporter::getVertex(MFnMesh* fMesh, MStatus& status)
{
	MPointArray			fVertexArray;
	MColorArray			fColorArray;
	MFloatVectorArray	fNormalArray;
	MFloatVectorArray	fTangentArray;
	MFloatVectorArray	fBinormalArray;

	if (MStatus::kFailure == fMesh->getPoints(fVertexArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getPoints"); 
		status =  MStatus::kFailure;
		return NULL;
	}

	if (MStatus::kFailure == fMesh->getFaceVertexColors(fColorArray)) {
		MGlobal::displayError("MFnMesh::getFaceVertexColors"); 
		status =  MStatus::kFailure;
		return NULL;
	}

	if (MStatus::kFailure == fMesh->getNormals(fNormalArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getNormals"); 
		status =  MStatus::kFailure;
		return NULL;
	}
	MString fCurrentUVSetName;
	if (MStatus::kFailure == fMesh->getCurrentUVSetName(fCurrentUVSetName)) {
		MGlobal::displayError("MFnMesh::getCurrentUVSetName"); 
		status = MStatus::kFailure;
		return NULL;
	}

	if (MStatus::kFailure == fMesh->getTangents(fTangentArray, MSpace::kWorld, &fCurrentUVSetName)) {
		MGlobal::displayError("MFnMesh::getTangents");
		status =  MStatus::kFailure;
		return NULL;
	}

	if (MStatus::kFailure == fMesh->getBinormals(fBinormalArray, MSpace::kWorld, &fCurrentUVSetName)) {
		MGlobal::displayError("MFnMesh::getBinormals");
		status =  MStatus::kFailure;
		return NULL;
	}

	vec2* uvs = getUVs(fMesh, status, fCurrentUVSetName);
	return new Vertex();
}

vec2* MayaFileExporter::getUVs(MFnMesh* fMesh, MStatus& status, MString currentUVSet)
{
	MStringArray uvSetNames;
	if (MStatus::kFailure == fMesh->getUVSetNames(uvSetNames)) {
		MGlobal::displayError("MFnMesh::getUVSetNames"); 
		status =  MStatus::kFailure;
		return NULL;
	}

	unsigned int uvSetCount = uvSetNames.length();
	unsigned int i;

	UVSet* currUVSet = NULL;

	UVSet* activeSet = NULL;
	for (i = 0; i < uvSetCount; i++ ) {
		if (0 == i) {
			currUVSet = new UVSet;
		} else {
			currUVSet->next = new UVSet;
			currUVSet = currUVSet->next;
		}

		currUVSet->name = uvSetNames[i];
		currUVSet->next = NULL;
		// Retrieve the UV values
		//
		if (MStatus::kFailure == fMesh->getUVs(currUVSet->uArray, currUVSet->vArray, &currUVSet->name)) {
			status = MStatus::kFailure;
			return NULL;
		}

		if(currUVSet->name == currentUVSet)
		{
			activeSet = currUVSet;
			break;
		}
	}

	vec2* uv = new vec2[activeSet->uArray.length()];

	for(uint i = 0; i < activeSet->uArray.length();i++)
	{
		uv[i] = vec2((float)activeSet->uArray[i], (float)activeSet->vArray[i]);
	}
	return uv;
}