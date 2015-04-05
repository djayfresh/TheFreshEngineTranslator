#include "stdafx.h"
#include "CppUnitTest.h"
#include "FileIO\BinaryFileReader.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testing
{		
	TEST_CLASS(ReadTests)
	{
	public:
		
		TEST_METHOD(ReadFile)
		{
			std::ifstream file("test1.bin", std::ios::binary | std::ios::in);
			file.seekg(0);
			BinaryFileReader reader;
			FileHeader fileHeader = reader.readFileHeader(file);
			Assert::AreEqual(0, (int)fileHeader.fileType);

			file.close();
		}
	};
}