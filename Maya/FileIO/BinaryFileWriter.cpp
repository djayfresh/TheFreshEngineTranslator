#include "BinaryFileWriter.h"
#include <iostream>

bool BinaryFileWirter::writeData(std::ostream& stream,const FileHeader& data)
{
	stream << data.fileType;
	stream << data.meshes;
	stream << data.lights;
	stream << data.textures;
	stream << data.additionalHeaders;

	return true;
}

bool BinaryFileWirter::writeData(std::ostream& stream, const MeshData& data)
{
	stream << data.shapeName;
	stream << data.numberOfVerts;
	stream << data.numberOfIndices;
	stream << data.numberOfTransforms;
	stream.write((char*)data.verts, sizeof(Vertex) * data.numberOfVerts);
	stream.write((char*)data.indices, sizeof(unsigned int) * data.numberOfIndices);

	stream.write((char*)data.transforms, sizeof(GeometryTransform) * data.numberOfTransforms);

	return true;
}

bool BinaryFileWirter::writeData(std::ostream& stream,const MeshDataHeader& data)
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


bool BinaryFileWirter::writeData(std::ostream& stream,const Light& data)
{
	stream << data.position;
	stream << data.color;
	stream << data.intensity;

	return false;
}
bool BinaryFileWirter::writeData(std::ostream& stream,const LightsHeader& data)
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
bool BinaryFileWirter::writeData(std::ostream& stream,const GeometryTransform& data)
{
	stream << data.geometryName;
	stream << data.position;
	stream << data.rotation;
	stream << data.scale;

	return false;
}
bool BinaryFileWirter::writeData(std::ostream& stream,const Texture& data)
{
	stream << data.textureName;
	stream << data.textureDataLength;
	stream.write((char*)data.textureData, data.textureDataLength);
	stream << data.width;
	stream << data.height;

	return true;
}
bool BinaryFileWirter::writeData(std::ostream& stream,const TextureHeader& data)
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

bool BinaryFileWirter::writeData(std::ostream& stream, Header* data)
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

bool BinaryFileWirter::writeData(std::ostream& stream,const AdditionalHeader& data)
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