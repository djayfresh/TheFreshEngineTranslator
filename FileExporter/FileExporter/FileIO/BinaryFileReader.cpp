#include "BinaryFileReader.h"

FileHeader BinaryFileReader::readFileHeader(std::ifstream& stream)
{
	char* buf = new char[sizeof(FileHeader)];
	stream.read(buf, sizeof(FileHeader));
	/*Type will one day be used for different header versions*/
	FileHeader file = *r_c(FileHeader*, buf);

	/*Find the location of each pointer within the file*/
	uint meshP = (uint)(file.meshes);
	uint lightP = (uint)(file.lights);
	uint textureP = (uint)(file.textures);
	uint additionalP = (uint)(file.additionalHeaders);

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
	char* buf = new char[sizeof(Light)];
	stream.read(buf, sizeof(Light));
	return r_c(Light*, buf);
}
LightsHeader* BinaryFileReader::readLightsHeader(std::ifstream& stream)
{
	char* buf = new char[sizeof(LightsHeader)];
	stream.read(buf, sizeof(LightsHeader));
	LightsHeader* header = r_c(LightsHeader*, buf);
	if(header->numberOfLights > 0)
	{
		stream.seekg((uint)header->lights);
		char* buff = new char[sizeof(Light) * header->numberOfLights];
		stream.read(buff, sizeof(Light) * header->numberOfLights);
		header->lights = r_c(Light*, buff);
	}
	else
	{
		header->lights = r_c(Light*, NULL);
	}

	return header;
}
	
AdditionalHeader* BinaryFileReader::readAdditionalHeaders(std::ifstream& stream)
{
	char* buff = new char[sizeof(AdditionalHeader)];
	stream.read(buff, sizeof(AdditionalHeader));
	AdditionalHeader* header = r_c(AdditionalHeader*, buff);

	stream.seekg((uint)header->headers);
	header->headers = readHeader(stream, header);

	return header;
}
	
Texture* BinaryFileReader::readTexture(std::ifstream& stream)
{
	char* buff = new char[sizeof(Texture)];

	stream.read(buff, sizeof(Texture));
	Texture* texture = r_c(Texture*, buff);

	if(texture->textureDataLength > 0)
	{
		stream.seekg((uint)texture->textureData);
		char* data = new char[texture->textureDataLength];
		stream.read(data, texture->textureDataLength);

		texture->textureData = r_c(void*,data);
	}
	else
	{
		texture->textureData = r_c(void*, NULL);
	}
	if(texture->nameLength > 0)
	{
		stream.seekg((uint)texture->textureName);

		char* data = new char[texture->nameLength];
		stream.read(data, texture->nameLength);

		texture->textureName = data;
	}
	else
	{
		texture->textureName = r_c(const char*, NULL);
	}
	return texture;
}
TextureHeader* BinaryFileReader::readTextureHeader(std::ifstream& stream)
{
	char* buf = new char[sizeof(TextureHeader)];
	stream.read(buf, sizeof(TextureHeader));
	TextureHeader* textureHandler = r_c(TextureHeader*, buf);

	if(textureHandler->numberOfTextures > 0)
	{
		stream.seekg((uint)textureHandler->textures);
		Texture* textures = new Texture[textureHandler->numberOfTextures];
		Texture* tmp;
		for(unsigned int i = 0; i < textureHandler->numberOfTextures; i++)
		{
			memcpy(&textures[i],tmp = readTexture(stream), sizeof(Texture));
			delete tmp;
		}
		textureHandler->textures = textures;
	}
	else
	{
		textureHandler->textures = r_c(Texture*, NULL);
	}
	return textureHandler;
}

Header** BinaryFileReader::readHeader(std::ifstream& stream, const AdditionalHeader* header)
{
	if(header->numberOfAdditonalHeaders > 0)
	{
		uint headerStart = (uint) stream.tellg();
		char* headerPrt = new char[sizeof(Header*) * header->numberOfAdditonalHeaders];

		stream.seekg((uint)header->headers);
		stream.read(headerPrt, sizeof(Header*) * header->numberOfAdditonalHeaders);
		Header** headers = r_c(Header**, headerPrt);
		char* headerTypes = new char[sizeof(uint)];
		uint headerLocation;
		for(int i = 0; i < header->numberOfAdditonalHeaders; i++)
		{
			headerLocation = (uint)headers[i];
			stream.seekg(headerLocation);

			stream.read(headerTypes, sizeof(Header));
			stream.seekg(headerLocation);
			Header* headerTmp = r_c(Header*, headerTypes);
			uint headerType = headerTmp->type;
			switch(headerType)
			{
			case HeaderTypes::Mesh:
				headers[i] = readMeshDataHeader(stream);
				break;
			case HeaderTypes::Textures:
				headers[i] = readTextureHeader(stream);
				break;
			case HeaderTypes::Lights:
				headers[i] = readLightsHeader(stream);
				break;
			case HeaderTypes::Additional:
				headers[i] = readAdditionalHeaders(stream);
				break;
			default:
				std::cout << "Header doesn't excist" << std::endl;
				break;
			}
		}
		delete headerTypes;
		return headers;
	}
	return NULL;
}