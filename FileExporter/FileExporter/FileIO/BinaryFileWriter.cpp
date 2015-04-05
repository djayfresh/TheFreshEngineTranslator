#include "BinaryFileWriter.h"
#include <iostream>
#include <fstream>

bool BinaryFileWriter::writeData(std::ofstream& stream,const FileHeader& data)
{
	int fileStart = stream.tellp();

	/*Move to the end of the FileHeader*/
	stream.seekp(fileStart + sizeof(FileHeader));

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

	FileHeader tmp(data.fileType, r_c(MeshDataHeader*,meshDataP), r_c(LightsHeader*,lightsP),
		r_c(TextureHeader*, textureP), r_c(AdditionalHeader*, additonalP));

	/*Write locations in file to pointers*/
	stream.seekp(fileStart);
	stream.write(r_c(char*,&tmp), sizeof(FileHeader));
	stream.seekp(fileEnd);
	return true;
}

bool BinaryFileWriter::writeData(std::ofstream& stream, const MeshData& data)
{
	MeshData header = data;
	uint headerStart = (uint)stream.tellp();
	stream.seekp(headerStart + sizeof(MeshData));

	header.verts = r_c(Vertex*, (uint) stream.tellp());
	stream.write(r_c(char*,data.verts), sizeof(Vertex) * data.numberOfVerts);

	header.indices = r_c(uint*, (uint) stream.tellp());
	stream.write(r_c(char*,data.indices), sizeof(unsigned int) * data.numberOfIndices);

	header.transforms = r_c(GeometryTransform*, (uint) stream.tellp());
	bool success = false;
	for(unsigned int i = 0; i < data.numberOfTransforms; i++)
	{
		success = writeData(stream, data.transforms[i]);
		if(!success)
		{
			return false;
		}
	}
	header.shapeName = r_c(const char*, (uint) stream.tellp());
	writeData(stream, data.shapeName);

	uint end = (uint) stream.tellp();

	stream.seekp(headerStart);
	stream.write(r_c(char*, &header), sizeof(MeshData));
	stream.seekp(end);

	return true;
}

bool BinaryFileWriter::writeData(std::ofstream& stream,const MeshDataHeader& data)
{
	MeshDataHeader header = data;
	uint headerStart = (uint) stream.tellp();

	stream.seekp(headerStart + sizeof(MeshDataHeader));
	header.meshes = r_c(MeshData*, (uint)stream.tellp());
	bool success = false;
	for(unsigned int i = 0; i < data.numberOfMeshes; i++)
	{
		success = writeData(stream, data.meshes[i]);
		if(!success)
		{
			return false;
		}
	}
	uint end = (uint) stream.tellp();

	stream.seekp(headerStart);
	stream.write(r_c(char*,&header), sizeof(MeshDataHeader));
	stream.seekp(end);

	return true;
}


bool BinaryFileWriter::writeData(std::ofstream& stream,const Light& data)
{
	Light light = data;
	stream.write(r_c(char*,&light), sizeof(Light));
	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const LightsHeader& data)
{
	LightsHeader header = data;
	uint headerStart = (uint) stream.tellp();

	stream.seekp(headerStart + sizeof(LightsHeader));
	header.lights = r_c(Light*, (uint) stream.tellp());
	bool success = false;
	for(unsigned int i = 0; i < data.numberOfLights; i++)
	{
		success = writeData(stream, data.lights[i]);
		if(!success)
		{
			return false;
		}
	}
	uint end = (uint)stream.tellp();


	stream.seekp(headerStart);
	stream.write(r_c(char*,&header), sizeof(LightsHeader));
	stream.seekp(end);

	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const GeometryTransform& data)
{
	GeometryTransform geo = data;
	uint headerStart = (uint) stream.tellp();

	stream.seekp(headerStart + sizeof(GeometryTransform));

	geo.geometryName = r_c(const char*, (uint)stream.tellp());
	writeData(stream, data.geometryName);

	uint end = (uint) stream.tellp();

	stream.seekp(headerStart);
	stream.write(r_c(char*, &geo), sizeof(GeometryTransform));
	stream.seekp(end);

	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const Texture& data)
{
	Texture text = data;
	uint headerStart = (uint) stream.tellp();

	//Write pointers at end of header
	stream.seekp(headerStart + sizeof(Texture));

	text.textureData = r_c(void*, (uint)stream.tellp());
	stream.write(r_c(char*,data.textureData), data.textureDataLength);

	text.textureName = r_c(const char*, (uint)stream.tellp());
	writeData(stream, data.textureName);

	uint end = (uint) stream.tellp();

	//Write header
	stream.seekp(headerStart);
	stream.write(r_c(char*, &text), sizeof(Texture));
	stream.seekp(end);

	return true;
}
bool BinaryFileWriter::writeData(std::ofstream& stream,const TextureHeader& data)
{
	TextureHeader header = data;
	uint headerStart = (uint) stream.tellp();

	stream.seekp(headerStart + sizeof(TextureHeader));

	header.textures = r_c(Texture*, (uint)stream.tellp());

	bool success = false;
	for(unsigned int i =0; i < data.numberOfTextures; i++)
	{
		success = writeData(stream, data.textures[i]);
		if(!success)
		{
			return false;
		}
	}

	uint end = stream.tellp();

	stream.seekp(headerStart);
	stream.write(r_c(char*, &header), sizeof(TextureHeader));
	stream.seekp(end);

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
	stream.write(r_c(const char*,&data.type), sizeof(unsigned int));
	stream.write(r_c(const char*,&data.numberOfAdditonalHeaders), sizeof(unsigned int));
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

bool BinaryFileWriter::writeData(std::ofstream& stream, const char* name)
{
	stream.write(name, strlen(name));
	return true;
}