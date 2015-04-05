#ifndef BINARY_FILE_READER_H
#define BINARY_FILE_READER_H

#include "../ExportHeader.h"
#include "FileReader.h"

class DLL_SHARED BinaryFileReader : public FileReader
{
public:
	FileHeader readFileHeader(std::ifstream& stream);
	
	MeshData* readMeshData(std::ifstream& stream);
	GeometryTransform* readGeometryTransform(std::ifstream& stream);
	MeshDataHeader* readMeshDataHeader(std::ifstream& stream);
	
	Light* readLight(std::ifstream& stream);
	LightsHeader* readLightsHeader(std::ifstream& stream);
	
	AdditionalHeader* readAdditionalHeaders(std::ifstream& stream);
	
	Texture* readTexture(std::ifstream& stream);
	TextureHeader* readTextureHeader(std::ifstream& stream);
	
	Header** readHeader(std::ifstream& stream, const AdditionalHeader* header);
};

#endif /* BINARY_FILE_READER_H*/