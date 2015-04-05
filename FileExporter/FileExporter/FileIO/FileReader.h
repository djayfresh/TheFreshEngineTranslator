#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <fstream>
#include "Headers.h"

#ifndef r_c(type,src)

#define r_c(type,src) reinterpret_cast<type>(src)

#endif

class FileReader
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
};

#endif /* FILE_READER_H */