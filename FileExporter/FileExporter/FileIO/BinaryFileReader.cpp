#include "BinaryFileReader.h"

FileHeader BinaryFileReader::readFileHeader(std::ifstream& stream)
{
	char* buf = new char[sizeof(FileHeader)];
	stream.read(buf, sizeof(FileHeader));
	FileHeader file;
	/*Type will one day be used for different header versions*/
	file.fileType = *r_c(unsigned int*, buf);
	/*Find the location of each pointer within the file*/
	int meshP = *r_c(unsigned int*, buf + FileHeader::MeshHeaderOffset);
	int lightP = *r_c(unsigned int*, buf + FileHeader::LightHeaderOffset);
	int textureP = *r_c(unsigned int*, buf + FileHeader::TextureHeaderOffset);
	int additionalP = *r_c(unsigned int*, buf + FileHeader::AdditionalHeaderOffset);

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
	/*Names is going to offset the file!! O.O*/
	char* nameBuff = new char[16];
	stream.getline(nameBuff, 16);
	/*Only read to where verts is written*/
	char* buf = new char[MeshData::Vertex_Offset];
	stream.read(buf, sizeof(MeshData));
	MeshData* mesh = new MeshData();
	mesh->shapeName = nameBuff;
	mesh->numberOfIndices = *r_c(unsigned int*,buf + MeshData::Num_Of_Indices_Offset);
	mesh->numberOfVerts = *r_c(unsigned int*,buf + MeshData::Num_Of_Verts_Offset);
	mesh->numberOfTransforms = *r_c(unsigned int*,buf + MeshData::Num_Of_Transforms_Offset);
	delete buf;
	/*Vertex*/
	int buffSize = sizeof(Vertex) * mesh->numberOfVerts;
	char* buffer = new char[buffSize];
	stream.read(buffer, buffSize);
	mesh->verts = r_c(Vertex*, buffer);
	/*Indices*/
	buffSize = sizeof(unsigned int) * mesh->numberOfIndices;
	buffer = new char[buffSize];
	stream.read(buffer, buffSize);
	mesh->indices = r_c(unsigned int*, buffer);

	/*Transform*/
	GeometryTransform* transforms = new GeometryTransform[mesh->numberOfTransforms];
	GeometryTransform* tmp;
	for(unsigned int i = 0; i < mesh->numberOfTransforms; i++)
	{
		memcpy(&transforms[i], tmp = readGeometryTransform(stream), sizeof(GeometryTransform));
		delete tmp;
	}
	buffer = new char[buffSize];
	stream.read(buffer, buffSize);
	mesh->indices = r_c(unsigned int*, buffer);

	return mesh;
}

GeometryTransform* BinaryFileReader::readGeometryTransform(std::ifstream& stream)
{
	char* name = new char[16];
	stream.getline(name, 16);
	GeometryTransform* geo = new GeometryTransform();
	geo->geometryName = name;
	/*Remove the pointer for name*/
	char* buff = new char[sizeof(GeometryTransform) - 4];
	stream.read(buff, sizeof(GeometryTransform) - 4);

	geo->position = *r_c(vec3*, buff + GeometryTransform::Position_Offset);
	geo->rotation = *r_c(vec3*, buff + GeometryTransform::Rotation_Offset);
	geo->scale = *r_c(vec3*, buff + GeometryTransform::Scale_Offset);

	delete buff;

	return geo;
}
MeshDataHeader* BinaryFileReader::readMeshDataHeader(std::ifstream& stream)
{
	char* buf = new char[sizeof(MeshDataHeader)];
	stream.read(buf, sizeof(MeshDataHeader));
	MeshDataHeader* meshHeader = new MeshDataHeader();
	unsigned int type = *r_c(unsigned int*, buf);
	if(type != meshHeader->type)
	{
		std::cout << "Wrong header type - MeshDataHeader: "<< stream.tellg() << std::endl;
		return NULL;
	}
	meshHeader->numberOfMeshes = *r_c(unsigned int*, buf + MeshDataHeader::Num_Of_Meshes_Offset);
	unsigned int meshP = *r_c(unsigned int*, buf + MeshDataHeader::Mesh_Data_Offset);
	stream.seekg(meshP);
	MeshData* meshData = new MeshData[meshHeader->numberOfMeshes];
	MeshData* tmp;
	for(unsigned int i = 0; i < meshHeader->numberOfMeshes; i++)
	{
		memcpy(&meshData[i],tmp = readMeshData(stream), sizeof(MeshData));
		delete tmp;
	}
	meshHeader->meshes = meshData;
	delete buf;
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