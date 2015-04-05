#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include "Headers.h"

class FileReader
{
public:
	virtual FileHeader readFileHeader(std::ostream& stream) = 0;
	
	virtual MeshData readMeshData(std::ostream& stream) = 0;
	virtual GeometryTransform readGeometryTransform(std::ostream& stream) = 0;
	virtual MeshDataHeader readMeshDataHeader(std::ostream& stream) = 0;
	
	virtual Light readLight(std::ostream& stream) = 0;
	virtual LightsHeader readLightsHeader(std::ostream& stream) = 0;
	
	virtual AdditionalHeader readAdditionalHeaders(std::ostream& stream) = 0;
	
	virtual Texture readTexture(std::ostream& stream) = 0;
	virtual TextureHeader readTextureHeader(std::ostream& stream) = 0;
};

#endif /* FILE_READER_H */