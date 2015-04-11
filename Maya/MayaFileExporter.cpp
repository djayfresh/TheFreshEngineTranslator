#include "MayaFileExporter.h"

#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnLight.h>
#include <maya/MPxTransformationMatrix.h>
#include <vector>

vec3 toVec3(const MFloatVector& vector)
{
	return vec3(vector.x, vector.y, vector.z);
}
vec4 toVec4(const MColor& vector)
{
	return vec4(vector.r, vector.g, vector.b, vector.a);
}

MeshData* MayaFileExporter::getMeshData(MDagPath& path, MStatus& status)
{
	MDagPath* fDagPath = new MDagPath(path);
	MFnMesh* fMesh = new MFnMesh(*fDagPath, &status);
	fDagPath->extendToShape();
	MFnDagNode dagNode(*fDagPath, &status);
	if (!status && status.statusCode () != MStatus::kSuccess)
        return NULL;

	MeshData* mesh = new MeshData();
	uint numberOf;
	mesh->verts = getVertex(fMesh, status, numberOf);
	mesh->numberOfVerts = numberOf;
	mesh->indices = getIndices(fMesh, status, numberOf);
	mesh->numberOfIndices = numberOf;
	mesh->transforms = getTransform(path, status);
	mesh->numberOfTransforms = 1;
	mesh->shapeName = dagNode.name().asChar();
	mesh->nameLength = stringLength(mesh->shapeName);
	
	if (!status && status.statusCode () != MStatus::kSuccess)
        return NULL;

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
	path.pop();
	MFnDagNode dagNode(path, &status);
	if (!status && status.statusCode () == MStatus::kInvalidParameter)
        return NULL;

	transform->geometryName = dagNode.name().asChar();
	transform->nameLength = stringLength(transform->geometryName);

	return transform;
}

Vertex* MayaFileExporter::getVertex(MFnMesh* fMesh, MStatus& status, uint& numberOfVerts)
{
	MPointArray			fVertexArray;
	MColorArray			fColorArray;
	MFloatVectorArray	fNormalArray;
	MFloatVectorArray	fTangentArray;
	MFloatVectorArray	fBinormalArray;
	bool colorSetMissing = false;

	if (MStatus::kFailure == fMesh->getPoints(fVertexArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getPoints"); 
		status =  MStatus::kFailure;
		return NULL;
	}
	
	MString colorSetName;
	if (MStatus::kFailure == fMesh->getCurrentColorSetName(colorSetName)) {
		MGlobal::displayError("MFnMesh::getFaceVertexColors"); 
		status =  MStatus::kFailure;
		return NULL;
	}

	if (MStatus::kFailure == fMesh->getFaceVertexColors(fColorArray, &colorSetName)) {
		MGlobal::displayError("MFnMesh::getFaceVertexColors"); 
		status =  MStatus::kFailure;
		colorSetMissing = true;
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

	uint numberOfUVs;
	vec2* uvs = getUVs(fMesh, status, fCurrentUVSetName, numberOfUVs);
	Vertex* vertex = new Vertex[fVertexArray.length()];
	for(uint i =0; i < fVertexArray.length(); i++)
	{
		if(i <= numberOfUVs)
		{
			vertex[i].uv = uvs[i];
		}
		if(i <= fBinormalArray.length())
		{
			vertex[i].bitangent = toVec3(fBinormalArray[i]);
		}
		if(i <= fNormalArray.length())
		{
			vertex[i].normal = toVec3(fNormalArray[i]);
		}
		if(i <= fVertexArray.length())
		{
			vertex[i].position = toVec3(fVertexArray[i]);
		}
		if(i <= fTangentArray.length())
		{
			vertex[i].tangent = toVec3(fTangentArray[i]);
		}

		//Colors
		if(colorSetMissing)
		{
			vertex[i].color = vec4((float)fVertexArray[i].x, (float)fVertexArray[i].y, (float)fVertexArray[i].z, 1);
		}
		else if(i <= fColorArray.length())
		{
			vertex[i].color = toVec4(fColorArray[i]);
		}
		//
	}
	numberOfVerts = fVertexArray.length();
	return vertex;
}

vec2* MayaFileExporter::getUVs(MFnMesh* fMesh, MStatus& status, MString currentUVSet, uint& numberOfUVs)
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
	numberOfUVs = activeSet->uArray.length();
	return uv;
}

uint* MayaFileExporter::getIndices(MFnMesh* fMesh, MStatus& status, uint& numberOfIndices)
{

	unsigned int faceCount = fMesh->numPolygons();
	if (0 == faceCount) {
		status = MStatus::kFailure;
		return NULL;
	}

	MIntArray indexArray;
	MIntArray allIndexArray;
	unsigned int i;
	for (i = 0; i < faceCount; i++) {

		unsigned int indexCount = fMesh->polygonVertexCount(i, &status);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MFnMesh::polygonVertexCount");
				status = MStatus::kFailure;
				return NULL;
		}

		status = fMesh->getPolygonVertices (i, indexArray);
		if (MStatus::kFailure == status) {
			MGlobal::displayError("MFnMesh::getPolygonVertices");
				status = MStatus::kFailure;
				return NULL;
		}
		for (uint j = 0; j < indexArray.length(); j++) {
			allIndexArray.append(indexArray[j]);
		}
	}
	
	uint* indices = new uint[allIndexArray.length()];
	for (uint j = 0; j < allIndexArray.length(); j++) {
		indices[j] = allIndexArray[j];
	}
	numberOfIndices = allIndexArray.length();
	return indices;
}

MStatus MayaFileExporter::parseScene(bool selection)
{
	cleanUp();
	MStatus status;
	status = parseMeshes(selection);
	if(!status && status != MStatus::kFailure)
	{
		MGlobal::displayError("Exporter::parseMeshes");
	}
	status = parseLights(selection);
	if(!status && status != MStatus::kFailure)
	{
		MGlobal::displayError("Exporter::parseLights");
	}
	return status;
}

MStatus MayaFileExporter::parseMeshes(bool selection)
{
	MStatus status;
	MItDag dag(MItDag::kDepthFirst, MFn::kMesh, &status);
	if(!status)
	{
		status.perror("MItDag::dagIterator");
		return status;
	}
	std::pair<const char*, MeshData*> pair;
	MeshData* mesh;
	MeshData* tmp;
	for(;!dag.isDone(); dag.next())
	{
		MDagPath dagPath;
		status = dag.getPath(dagPath);
		if(!status)
		{
			status.perror("MItDag::getPath");
			continue;
		}

		MFnDagNode dagNode(dagPath, &status);
		if(!status)
		{
			status.perror("MFnDagNode constructor");
			continue;
		}

		cout << dagNode.name().asChar() << ": " << dagNode.typeName().asChar() << endl;
		cout << " dagPath: " << dagPath.fullPathName().asChar() << endl;
		cout << endl;
		mesh = getMeshData(dagPath, status);
		pair = std::make_pair(mesh->shapeName, mesh);
		if(meshes.count(mesh->shapeName) > 0)
		{
			tmp = meshes[mesh->shapeName];
			uint transformCount = tmp->numberOfTransforms + mesh->numberOfTransforms;
			GeometryTransform* transform = new GeometryTransform[transformCount];
			uint i;
			for(i = 0; i < tmp->numberOfTransforms; i++)
			{
				transform[i] = tmp->transforms[i];
			}
			for(uint j = 0; j < mesh->numberOfTransforms && i < transformCount; j++, i++)
			{
				transform[i] = mesh->transforms[j];
			}
			delete tmp->transforms;
			tmp->transforms = transform;
			tmp->numberOfTransforms = transformCount;
		}
		else
		{
			meshes.emplace(pair);
		}
	}

	return status;
}

MStatus MayaFileExporter::parseLights(bool selection)
{
	MStatus status;
	MItDag dag(MItDag::kDepthFirst, MFn::kLight, &status);
	if(!status)
	{
		status.perror("MItDag::dagIterator");
		return status;
	}
	for(;!dag.isDone(); dag.next())
	{
		MDagPath dagPath;
		status = dag.getPath(dagPath);
		if(!status)
		{
			status.perror("MItDag::getPath");
			continue;
		}

		MFnDagNode dagNode(dagPath, &status);
		if(!status)
		{
			status.perror("MFnDagNode constructor");
			continue;
		}

		cout << dagNode.name().asChar() << ": " << dagNode.typeName().asChar() << endl;
		cout << " dagPath: " << dagPath.fullPathName().asChar() << endl;
		cout << endl;
		lights.push_back(getLights(dagPath, status));
	}

	return status;
}

void deleteMeshData(MeshData* mesh)
{
	delete mesh->transforms;
	delete mesh->verts;
	delete mesh->indices;
}

MStatus MayaFileExporter::cleanUp()
{
	MeshData* mesh;
	for (std::map<const char*,MeshData*, cmp_str>::iterator it=meshes.begin(); it!=meshes.end(); ++it)
	{
		mesh = meshes[it->first];
		deleteMeshData(mesh);
		delete mesh;
		meshes[it->first] = NULL;
	}
	meshes.clear();
	uint i;
	for(i = 0; i < lights.size(); i++)
	{
		delete lights[i];
		lights[i] = NULL;
	}
	lights.clear();
	for(i = 0; i < textures.size(); i++)
	{
		delete textures[i];
		textures[i] = NULL;
	}
	textures.clear();

	return MStatus::kSuccess;
}

Light* MayaFileExporter::getLights(MDagPath& path, MStatus& status)
{
	Light* light = new Light();
	MFnLight dag(path, &status);
	if(!status)
	{
		status.perror("MFnLight constructor");
	}
	GeometryTransform* transform = getTransform(path, status);
	light->position = transform->position;
	light->intensity = toVec4(dag.lightIntensity(&status));
	light->color = toVec4(dag.color());
	return light;
}
Texture* MayaFileExporter::getTexture(MDagPath& path, MStatus& status)
{
	return NULL;
}

MStatus MayaFileExporter::writeFile()
{
	MStatus status;
	BinaryFileWriter writer;

	FileHeader header;
	header.fileType = 0;
	header.meshes = createMeshHeader();
	header.additionalHeaders = createAdditionalHeaders();
	header.lights = createLightsHeader();
	header.textures = createTextureHeader();
	writer.writeData(file, header);
	file.flush();
	file.close();

	cleanup(header);
	//cleanUp();
	return status;
}

MeshDataHeader* MayaFileExporter::createMeshHeader()
{
	MeshDataHeader* header = new MeshDataHeader();
	MeshData* mesh = new MeshData[meshes.size()];
	uint i = 0;
	std::map<const char*,MeshData*, cmp_str>::iterator it;
	for (it=meshes.begin(); it!=meshes.end();i++, ++it)
	{
		memcpy(&mesh[i], meshes[it->first], sizeof(MeshData));
	}
	header->meshes = mesh;
	header->numberOfMeshes = (uint)meshes.size();

	return header;
}
AdditionalHeader* MayaFileExporter::createAdditionalHeaders()
{
	return new AdditionalHeader();
}
LightsHeader* MayaFileExporter::createLightsHeader()
{
	Light* lightsList = new Light[lights.size()];
	uint i;
	for(i = 0; i < lights.size(); i++)
	{
		lightsList[i] = *lights[i];
	}
	LightsHeader* header = new LightsHeader();
	header->lights = lightsList;
	header->numberOfLights = (uint)lights.size();
	return header;
}

TextureHeader* MayaFileExporter::createTextureHeader()
{
	return new TextureHeader();
}

void MayaFileExporter::cleanup(FileHeader& header)
{
	uint i;
	for(i = 0; i < header.meshes->numberOfMeshes; i++)
	{
		deleteMeshData(&header.meshes->meshes[i]);
	}
	if(header.meshes->meshes != NULL)
	{
		delete [] header.meshes->meshes;
	}
	if(header.lights->lights != NULL)
	{
		delete [] header.lights->lights;
	}
	if(header.additionalHeaders->headers != NULL)
	{
		delete [] header.additionalHeaders->headers;
	}

	delete header.meshes;
	delete header.additionalHeaders;
	delete header.lights;
	delete header.textures;
}
