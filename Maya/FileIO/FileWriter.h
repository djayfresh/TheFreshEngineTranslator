#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <iostream>
#include "Headers.h"

class FileWriter
{
public:
	virtual bool writeData(std::ostream& stream,const FileHeader& data) = 0;
	
	virtual bool writeData(std::ostream& stream,const MeshData& data) = 0;
	virtual bool writeData(std::ostream& stream,const GeometryTransform& data) = 0;
	virtual bool writeData(std::ostream& stream,const MeshDataHeader& data) = 0;
	
	virtual bool writeData(std::ostream& stream,const Light& data) = 0;
	virtual bool writeData(std::ostream& stream,const LightsHeader& data) = 0;
	
	virtual bool writeData(std::ostream& stream,const Texture& data) = 0;
	virtual bool writeData(std::ostream& stream,const TextureHeader& data) = 0;
	
	virtual bool writeData(std::ostream& stream,const AdditionalHeader& data) = 0;
	virtual bool writeData(std::ostream& stream,Header* data) = 0;
};

#endif /* FILE_WIRTER_H */