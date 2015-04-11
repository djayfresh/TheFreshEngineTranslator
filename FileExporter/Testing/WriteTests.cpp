#include "stdafx.h"
#include "CppUnitTest.h"
#include "FileIO\BinaryFileWriter.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testing
{		
	TEST_CLASS(WriteTests)
	{
	public:
		
		TEST_METHOD(WriteFile)
		{
			std::ofstream file("test1.bin", std::ios::binary | std::ios::out);
			BinaryFileWriter writer;
			Assert::AreEqual(true, writer.writeData(file, createTestFile()));
			file.flush();
			file.close();
		}
	private:
		FileHeader createTestFile()
		{
			
			FileHeader newFileHeader;
			newFileHeader.fileType = 0;

			newFileHeader.meshes = createMeshHeader();
			newFileHeader.additionalHeaders = createAdditionalHeader();
			newFileHeader.lights = createLightsHeader();
			newFileHeader.textures = createTextureHeader();
			return newFileHeader;
		}

		MeshDataHeader* createMeshHeader()
		{
			MeshDataHeader* meshDataHeader = new MeshDataHeader();
			meshDataHeader->numberOfMeshes = 1;
			meshDataHeader->meshes = createMeshDataArray(meshDataHeader->numberOfMeshes);
			return meshDataHeader;
		}

		MeshData createMeshData(int indices, int transforms, int verts)
		{
			MeshData meshes;
			meshes.shapeName = "shape";
			meshes.nameLength = stringLength(meshes.shapeName);
			meshes.numberOfIndices = indices;
			meshes.numberOfTransforms = transforms;
			meshes.numberOfVerts = verts;
			meshes.transforms = createTransformArray(transforms);
			meshes.verts = createVertexArray(verts);
			meshes.indices = createIndicesArray(indices);
			return meshes;
		}

		MeshData* createMeshDataArray(int count)
		{
			MeshData* meshes = new MeshData[count];
			for(int i =0; i < count; i++)
			{
				meshes[i] = createMeshData(1, 2, 3);
			}
			return meshes;
		}

		unsigned int* createIndicesArray(int count)
		{
			unsigned int* indices = new unsigned int[count];
			for(int i =0; i < count; i++)
			{
				indices[i] = i;
			}
			return indices;
		}

		Vertex* createVertexArray(int count)
		{
			Vertex* verts = new Vertex[count];
			for(int i =0; i < count; i++)
			{
				Vertex* thisVert = &verts[count];
				thisVert->position = vec3(i * 1.0f,i * 1.0f,i * 1.0f);
				thisVert->uv = vec2(1,1);
			}
			return verts;
		}

		GeometryTransform* createTransformArray(int count)
		{
			GeometryTransform* transforms = new GeometryTransform[count];
			const char* name = "Geo_";
			for(int i = 0; i < count; i++)
			{
				transforms[i] = createTransform(name, vec3(i * 1.0f,i * 1.0f,i * 1.0f), vec3(i * 1.0f,i * 1.0f,i * 1.0f), vec3(i * 1.0f,i * 1.0f,i * 1.0f));
			}
			return transforms;
		}

		GeometryTransform createTransform(const char* name, vec3 pos, vec3 rot, vec3 scale)
		{
			GeometryTransform transform;
			transform.geometryName = name;
			transform.nameLength = stringLength(name);
			transform.position = pos;
			transform.rotation = rot;
			transform.scale = scale;
			return transform;
		}

		LightsHeader* createLightsHeader()
		{
			LightsHeader* lights = new LightsHeader();
			lights->numberOfLights = 3;
			lights->lights = createLightsArray(lights->numberOfLights);
			return lights;
		}

		Light* createLightsArray(int count)
		{
			Light* lights = new Light[count];
			for(int i = 0; i < count; i++)
			{
				lights[i].position = vec3(i * 1.0f, i * 1.0f, i * 1.0f);
			}
			return lights;
		}

		Texture* createTexturesArray(int count)
		{
			Texture* textures = new Texture[count];

			for(int i =0; i < count; i++)
			{
				textures[i].textureName = "AVeryCoolTexture.jpg";
				textures[i].nameLength = stringLength(textures[i].textureName);
				textures[i].height = i * 10;
				textures[i].width = i * 40;
				textures[i].textureDataLength = 0;
				textures[i].textureData = NULL;
			}

			return textures;
		}

		TextureHeader* createTextureHeader()
		{
			TextureHeader* header = new TextureHeader();
			header->numberOfTextures = 1;
			header->textures = createTexturesArray(header->numberOfTextures);

			return header; 
		}

		AdditionalHeader* createAdditionalHeader()
		{
			AdditionalHeader* header = new AdditionalHeader();
			header->numberOfAdditonalHeaders = 4;
			if(header->numberOfAdditonalHeaders > 0)
			{
				header->headers = new Header*[header->numberOfAdditonalHeaders];
				for(uint i = 0; i < header->numberOfAdditonalHeaders; i++)
				{
					if(i == 0)
					{
						header->headers[i] = createMeshHeader();
					}
					else if(i == 1)
					{
						header->headers[i] = createTextureHeader();
					}
					else
					{
						header->headers[i] = createLightsHeader();
					}
				}
			}
			else
			{
				header->headers = NULL;
			}

			return header;
		}
	};
}