#ifndef BINARY_FILE_WRITER_H
#define BINARY_FILE_WRITER_H

#include "FileWriter.h"

class BinaryFileWirter : public FileWriter
{
public:
	bool writeData(std::ostream& stream,const FileHeader& data);
	bool writeData(std::ostream& stream,const MeshData& data);
	bool writeData(std::ostream& stream,const Light& data);
	bool writeData(std::ostream& stream,const LightsHeader& data);
	bool writeData(std::ostream& stream,const MeshDataHeader& data);
	bool writeData(std::ostream& stream,const GeometryTransform& data);
	bool writeData(std::ostream& stream,const Texture& data);
	bool writeData(std::ostream& stream,const TextureHeader& data);
	bool writeData(std::ostream& stream,const AdditionalHeader& data);
	bool writeData(std::ostream& stream, Header* data);
};

#endif /*BINARY_FILE_WRITER_H*/