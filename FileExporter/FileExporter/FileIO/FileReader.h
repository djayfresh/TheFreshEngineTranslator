#ifndef FILE_READER_H
#define FILE_READER_H

#include "../ExportHeader.h"
#include <iostream>
#include <fstream>
#include "Headers.h"

#ifndef r_c
#define r_c(type,src) reinterpret_cast<type>(src)
#endif

class DLL_SHARED FileReader
{
public:
	virtual FileHeader readFileHeader(std::ifstream& stream) = 0;
	
	virtual MeshData* readMeshData(std::ifstream& stream) = 0;
	virtual GeometryTransform* readGeometryTransform(std::ifstream& stream) = 0;
	virtual MeshDataHeader* readMeshDataHeader(std::ifstream& stream) = 0;
	
	virtual Light* readLight(std::ifstream& stream) = 0;
	virtual LightsHeader* readLightsHeader(std::ifstream& stream) = 0;
	
	virtual AdditionalHeader* readAdditionalHeaders(std::ifstream& stream) = 0;
	
	virtual Texture* readTexture(std::ifstream& stream) = 0;
	virtual TextureHeader* readTextureHeader(std::ifstream& stream) = 0;

	virtual Header** readHeader(std::ifstream& stream, const AdditionalHeader* header) = 0;

};

#endif /* FILE_READER_H */