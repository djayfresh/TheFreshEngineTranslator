#ifndef BINARY_FILE_WRITER_H
#define BINARY_FILE_WRITER_H

#include "../ExportHeader.h"
#include "FileWriter.h"

class DLL_SHARED BinaryFileWriter : public FileWriter
{
public:

	bool writeData(std::ofstream& stream,const FileHeader& data);
	bool writeData(std::ofstream& stream,const MeshData& data);
	bool writeData(std::ofstream& stream,const Light& data);
	bool writeData(std::ofstream& stream,const LightsHeader& data);
	bool writeData(std::ofstream& stream,const MeshDataHeader& data);
	bool writeData(std::ofstream& stream,const GeometryTransform& data);
	bool writeData(std::ofstream& stream,const Texture& data);
	bool writeData(std::ofstream& stream,const TextureHeader& data);
	bool writeData(std::ofstream& stream,const AdditionalHeader& data);
	bool writeData(std::ofstream& stream, Header* data);
protected:
	bool writeData(std::ofstream& stream,const char* name);
};

#endif /*BINARY_FILE_WRITER_H*/