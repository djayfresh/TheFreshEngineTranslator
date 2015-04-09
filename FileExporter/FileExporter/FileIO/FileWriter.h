#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <iostream>
#include "Headers.h"
#include "../ExportHeader.h"

#ifndef r_c

#define r_c(type,src) reinterpret_cast<type>(src)

#endif


class DLL_SHARED FileWriter
{
public:
	virtual bool writeData(std::ofstream& stream,const FileHeader& data) = 0;

	virtual bool writeData(std::ofstream& stream,const MeshData& data) = 0;
	virtual bool writeData(std::ofstream& stream,const GeometryTransform& data) = 0;
	virtual bool writeData(std::ofstream& stream,const MeshDataHeader& data) = 0;

	virtual bool writeData(std::ofstream& stream,const Light& data) = 0;
	virtual bool writeData(std::ofstream& stream,const LightsHeader& data) = 0;

	virtual bool writeData(std::ofstream& stream,const Texture& data) = 0;
	virtual bool writeData(std::ofstream& stream,const TextureHeader& data) = 0;

	virtual bool writeData(std::ofstream& stream,const AdditionalHeader& data) = 0;
	virtual bool writeData(std::ofstream& stream,Header* data) = 0;
};

#endif /* FILE_WIRTER_H */