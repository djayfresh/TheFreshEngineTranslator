#include "BinaryFileReader.h"

FileHeader BinaryFileReader::readFileHeader(std::ifstream& stream)
{
	char* buf = new char[sizeof(FileHeader)];
	stream.read(buf, sizeof(FileHeader));
	/*Type will one day be used for different header versions*/
	FileHeader file = *r_c(FileHeader*, buf);

	/*Find the location of each pointer within the file*/
	uint meshP = (uint)( file.meshes);
	uint lightP = r_c(uint, file.lights);
	uint textureP = r_c(uint, file.textures);
	uint additionalP = r_c(uint, file.additionalHeaders);

	/*Move the stream to the proper header location, let it read the data*/
	stream.seekg(meshP);
	file.meshes = readMeshDataHeader(stream);
	stream.seekg(lightP);
	file.lights = readLightsHeader(stream);
	stream.seekg(textureP);
	file.textures = readTextureHeader(stream);
	stream.seekg(additionalP);
	file.additionalHeaders = readAdditionalHeaders(stream);

	delete buf;

	return file;
}
	
MeshData* BinaryFileReader::readMeshData(std::ifstream& stream)
{
	char* buf = new char[sizeof(MeshData)];
	stream.read(buf, sizeof(MeshData));

	MeshData* mesh = r_c(MeshData*, buf);

	/*Vertex*/
	stream.seekg((uint)mesh->verts);
	int buffSize = sizeof(Vertex) * mesh->numberOfVerts;
	char* buffer = new char[buffSize];
	stream.read(buffer, buffSize);
	mesh->verts = r_c(Vertex*, buffer);
	/*Indices*/
	stream.seekg((uint)mesh->indices);
	buffSize = sizeof(unsigned int) * mesh->numberOfIndices;
	buffer = new char[buffSize];
	stream.read(buffer, buffSize);
	mesh->indices = r_c(unsigned int*, buffer);

	/*Transform*/
	stream.seekg((uint)mesh->transforms);
	GeometryTransform* transforms = new GeometryTransform[mesh->numberOfTransforms];
	GeometryTransform* tmp;
	for(unsigned int i = 0; i < mesh->numberOfTransforms; i++)
	{
		memcpy(&transforms[i], tmp = readGeometryTransform(stream), sizeof(GeometryTransform));
		delete tmp;
	}
	mesh->transforms = transforms;

	buffer = new char[mesh->nameLength];
	stream.seekg((uint)mesh->shapeName);
	stream.read(buffer, mesh->nameLength);
	mesh->shapeName = buffer;
	
	return mesh;
}

GeometryTransform* BinaryFileReader::readGeometryTransform(std::ifstream& stream)
{
	char* buff = new char[sizeof(GeometryTransform)];

	stream.read(buff, sizeof(GeometryTransform));
	GeometryTransform* geo = r_c(GeometryTransform*, buff);

	stream.seekg((uint)geo->geometryName);

	char* data = new char[geo->nameLength];
	stream.read(data, geo->nameLength);

	geo->geometryName = data;

	return geo;
}
MeshDataHeader* BinaryFileReader::readMeshDataHeader(std::ifstream& stream)
{
	char* buf = new char[sizeof(MeshDataHeader)];
	stream.read(buf, sizeof(MeshDataHeader));
	MeshDataHeader* meshHeader = r_c(MeshDataHeader*, buf);

	stream.seekg((uint)meshHeader->meshes);
	MeshData* meshData = new MeshData[meshHeader->numberOfMeshes];
	MeshData* tmp;
	for(unsigned int i = 0; i < meshHeader->numberOfMeshes; i++)
	{
		memcpy(&meshData[i],tmp = readMeshData(stream), sizeof(MeshData));
		delete tmp;
	}
	meshHeader->meshes = meshData;

	return meshHeader;
}
	
Light* BinaryFileReader::readLight(std::ifstream& stream)
{
	return new Light();
}
LightsHeader* BinaryFileReader::readLightsHeader(std::ifstream& stream)
{
	return new LightsHeader();
}
	
AdditionalHeader* BinaryFileReader::readAdditionalHeaders(std::ifstream& stream)
{
	return new AdditionalHeader();
}
	
Texture* BinaryFileReader::readTexture(std::ifstream& stream)
{
	return new Texture();
}
TextureHeader* BinaryFileReader::readTextureHeader(std::ifstream& stream)
{
	return new TextureHeader();
}