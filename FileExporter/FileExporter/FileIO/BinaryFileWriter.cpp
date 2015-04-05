#include "BinaryFileWriter.h"
#include <iostream>
#include <fstream>



bool BinaryFileWriter::writeData(std::ofstream& stream,const FileHeader& data)
{
	stream << data.fileType;
	int meshP = stream.tellp();

	/*Move to the end of the FileHeader*/
	stream.seekp(sizeof(FileHeader) - sizeof(data.fileType));

	/*Write and store locations to each header*/
	int meshDataP = stream.tellp();
	writeData(stream, data.meshes);
	int lightsP = stream.tellp();
	writeData(stream, data.lights);
	int textureP = stream.tellp();
	writeData(stream, data.textures);
	int additonalP = stream.tellp();
	writeData(stream, data.additionalHeaders);
	int fileEnd = stream.tellp();

	/*Write locations in file to pointers*/
	stream.seekp(meshP);
	stream << meshDataP;
	stream << lightsP;
	stream << textureP;
	stream << additonalP;
	stream.seekp(fileEnd);
	return true;
}

bool BinaryFileWriter::writeData(std::ofstream& stream, const MeshData& data)
{
	stream.write(data.shapeName, strlen(data.shapeName));
	stream << data.numberOfVerts;
	stream << data.numberOfIndices;
	stream << data.numberOfTransforms;
	stream.write(r_c(char*,data.verts), sizeof(Vertex) * data.numberOfVerts);
	stream.write(r_c(char*,data.indices), sizeof(unsigned int) * data.numberOfIndices);

	bool success = false;
	for(unsigned int i = 0; i < data.numberOfTransforms; i++)
	{
		success = writeData(stream, data.transforms[i]);
		if(!success)
		{
			return false;
		}
	}
	if(data.numberOfTransforms == 0)
	{
		stream << NULL;
	}

	return true;
}

bool BinaryFileWriter::writeData(std::ofstream& stream,const MeshDataHeader& data)
{
	MeshDataHeader header = data;
	stream << data.type;
	stream << data.numberOfMeshes;
	bool success = false;
	for(unsigned int i = 0; i < data.numberOfMeshes; i++)
	{
		success = writeData(stream, data.meshes[i]);
		if(!success)
		{
			return false;
		}
	}
	if(data.numberOfMeshes == 0)
	{
		stream << NULL;
	}

	return true;
}


bool BinaryFileWriter::writeData(std::ofstream& stream,const Light& data)
{
	stream.write(r_c(const char*,&data.position), sizeof(vec3));
	stream.write(r_c(const char*,&data.color), sizeof(vec4));
	stream << data.intensity;

	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const LightsHeader& data)
{
	stream << data.type;
	stream << data.numberOfLights;
	bool success = false;
	for(unsigned int i = 0; i < data.numberOfLights; i++)
	{
		success = writeData(stream, data.lights[i]);
		if(!success)
		{
			return false;
		}
	}

	if(data.numberOfLights == 0)
	{
		stream << NULL;
	}

	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const GeometryTransform& data)
{
	stream.write(data.geometryName, strlen(data.geometryName));
	stream.write(r_c(const char*,&data.position), sizeof(vec3));
	stream.write(r_c(const char*,&data.rotation), sizeof(vec3));
	stream.write(r_c(const char*,&data.scale), sizeof(vec3));

	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const Texture& data)
{
	stream.write(data.textureName, strlen(data.textureName));
	stream << data.textureDataLength;
	stream.write(r_c(char*,data.textureData), data.textureDataLength);
	stream << data.width;
	stream << data.height;

	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const TextureHeader& data)
{
	stream << data.type;
	stream << data.numberOfTextures;
	bool success = false;
	for(unsigned int i =0; i < data.numberOfTextures; i++)
	{
		success = writeData(stream, data.textures[i]);
		if(!success)
		{
			return false;
		}
	}
	
	if(data.numberOfTextures == 0)
	{
		stream << NULL;
	}

	return true;
}

bool BinaryFileWriter::writeData(std::ofstream& stream, Header* data)
{
	switch(data->type)
	{
	case HeaderTypes::Mesh:
		writeData(stream, *(MeshDataHeader*)data);
		break;
	case HeaderTypes::Lights:
		writeData(stream, *(LightsHeader*)data);
		break;
	case HeaderTypes::Additional:
		writeData(stream, *(AdditionalHeader*)data);
		break;
	case HeaderTypes::Textures:
		writeData(stream, *(TextureHeader*)data);
		break;
	default:
		std::cout << "Header not recongnized" << std::endl;
		return false;
	}
	return true;
}

bool BinaryFileWriter::writeData(std::ofstream& stream,const AdditionalHeader& data)
{
	stream << data.type;
	stream << data.numberOfAdditonalHeaders;
	bool success = false;
	for(unsigned int i =0; i < data.numberOfAdditonalHeaders; i++)
	{
		success = writeData(stream, &data.headers[i]);
		if(!success)
		{
			return false;
		}
	}

	if(data.numberOfAdditonalHeaders == 0)
	{
		stream << NULL;
	}

	return true;
}