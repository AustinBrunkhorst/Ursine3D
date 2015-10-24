#pragma once

#include "UrsinePrecompiled.h"
#include "MeshInfo.h"
namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			MeshInfo::MeshInfo() :
				vertexCount(0), indexCount(0), normalCount(0), tangentCount(0), uvCount(0),
				mtrlIndexCount(0), mtrlCount(0), ctrlPtCount(0), subsetCount(0),
				normalMode(FbxLayerElement::EMappingMode::eNone),
				tangentMode(FbxLayerElement::EMappingMode::eNone),
				vertices(nullptr), indices(nullptr), normals(nullptr), tangents(nullptr), uvs(nullptr), materialIndices(nullptr),
				modelSubsets(nullptr), //fbxmaterials(nullptr),
				ctrlIndices(nullptr), ctrlBlendWeights(nullptr), ISerialize("")
			{
				*mtrlName = { nullptr };
			}

			MeshInfo::~MeshInfo()
			{
				ReleaseData();
			}

			void MeshInfo::ReleaseData()
			{
				if (vertices)
				{
					delete[] vertices;
					vertices = nullptr;
				}
				if (indices)
				{
					delete[] indices;
					indices = nullptr;
				}
				if (normals)
				{
					delete[] normals;
					normals = nullptr;
				}
				if (tangents)
				{
					delete[] tangents;
					tangents = nullptr;
				}
				if (uvs)
				{
					delete[] uvs;
					uvs = nullptr;
				}
				if (mtrlName)
				{
					for (unsigned int i = 0; i < mtrlCount; ++i)
						delete mtrlName[i];
				}
				if (materialIndices)
				{
					delete[] materialIndices;
					materialIndices = nullptr;
				}
				if (modelSubsets)
				{
					delete[] modelSubsets;
					modelSubsets = nullptr;
				}
				if (ctrlIndices)
				{
					for (unsigned int i = 0; i < 4; ++i)
					{
						delete[] ctrlIndices[i];
					}
					delete[] ctrlIndices;
				}
				if (ctrlBlendWeights)
				{
					for (unsigned int i = 0; i < 4; ++i)
					{
						delete[] ctrlBlendWeights[i];
					}
					delete[] ctrlBlendWeights;
				}
			}

			bool MeshInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				unsigned int i = 0, j = 0;

				if (hFile != INVALID_HANDLE_VALUE)
				{
					ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					ReadFile(hFile, &vertexCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &indexCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &normalCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &tangentCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &uvCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &ctrlPtCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &subsetCount, sizeof(unsigned int), &nBytesRead, nullptr);

					vertices = new XMFLOAT3[vertexCount];
					for (i = 0; i < vertexCount; ++i)
					{
						ReadFile(hFile, &vertices[i], sizeof(XMFLOAT3), &nBytesRead, nullptr);
					}
					indices = new unsigned int[indexCount];
					for (i = 0; i < indexCount; ++i)
					{
						ReadFile(hFile, &indices[i], sizeof(unsigned int), &nBytesRead, nullptr);
					}
					normals = new XMFLOAT3[normalCount];
					for (i = 0; i < normalCount; ++i)
					{
						ReadFile(hFile, &normals[i], sizeof(XMFLOAT3), &nBytesRead, nullptr);
					}
					tangents = new XMFLOAT3[tangentCount];
					for (i = 0; i < tangentCount; ++i)
					{
						ReadFile(hFile, &tangents[i], sizeof(XMFLOAT3), &nBytesRead, nullptr);
					}
					uvs = new XMFLOAT2[uvCount];
					for (i = 0; i < uvCount; ++i)
					{
						ReadFile(hFile, &uvs[i], sizeof(XMFLOAT2), &nBytesRead, nullptr);
					}
					// replace this fbxmaterial or just name of mtrl,texture
					//materials = new FBX_DATA::Material[mtrlCount];
					//for (i = 0; i < mtrlCount; ++i)
					//{
					//	ReadFile(hFile, &materials[i], sizeof(FBX_DATA::Material), &nBytesRead, nullptr);
					//}
					for (i = 0; i < mtrlCount; ++i)
					{
						mtrlName[i] = new char[MAXTEXTLEN];
						ReadFile(hFile, mtrlName[i], sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					}
					materialIndices = new unsigned int[mtrlIndexCount];
					for (i = 0; i < mtrlIndexCount; ++i)
					{
						ReadFile(hFile, &materialIndices[i], sizeof(unsigned int), &nBytesRead, nullptr);
					}
					ctrlIndices = new unsigned int*[ctrlPtCount];
					ctrlBlendWeights = new double*[ctrlPtCount];
					for (i = 0; i < ctrlPtCount; ++i)
					{
						ctrlIndices[i] = new unsigned int[4];
						ctrlBlendWeights[i] = new double[4];
						for (j = 0; j < 4; ++j)
						{
							ReadFile(hFile, &ctrlIndices[i][j], sizeof(unsigned int), &nBytesRead, nullptr);
						}
						for (j = 0; j < 4; ++j)
						{
							ReadFile(hFile, &ctrlBlendWeights[i][j], sizeof(double), &nBytesRead, nullptr);
						}
					}
					modelSubsets = new FBX_DATA::ModelSubset[subsetCount];
					for (i = 0; i < subsetCount; ++i)
					{
						ReadFile(hFile, &modelSubsets[i], sizeof(FBX_DATA::ModelSubset), &nBytesRead, nullptr);
					}
				}
				return true;
			}

			bool MeshInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				unsigned int i = 0, j = 0;

				if (hFile != INVALID_HANDLE_VALUE)
				{
					WriteFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					WriteFile(hFile, &vertexCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &indexCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &normalCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &tangentCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &uvCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &ctrlPtCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &subsetCount, sizeof(unsigned int), &nBytesWrite, nullptr);

					for (i = 0; i < vertexCount; ++i)
					{
						WriteFile(hFile, &vertices[i], sizeof(XMFLOAT3), &nBytesWrite, nullptr);
					}
					for (i = 0; i < indexCount; ++i)
					{
						WriteFile(hFile, &indices[i], sizeof(unsigned int), &nBytesWrite, nullptr);
					}
					for (i = 0; i < normalCount; ++i)
					{
						WriteFile(hFile, &normals[i], sizeof(XMFLOAT3), &nBytesWrite, nullptr);
					}
					for (i = 0; i < tangentCount; ++i)
					{
						WriteFile(hFile, &tangents[i], sizeof(XMFLOAT3), &nBytesWrite, nullptr);
					}
					for (i = 0; i < uvCount; ++i)
					{
						WriteFile(hFile, &uvs[i], sizeof(XMFLOAT2), &nBytesWrite, nullptr);
					}
					// replace this to fbxmaterial or just name of mtrl,texture
					//for (i = 0; i < mtrlCount; ++i)
					//{
					//	WriteFile(hFile, &materials[i], sizeof(FBX_DATA::Material), &nBytesWrite, nullptr);
					//}
					for (i = 0; i < mtrlCount; ++i)
					{
						WriteFile(hFile, mtrlName[i], sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					}
					for (i = 0; i < mtrlIndexCount; ++i)
					{
						WriteFile(hFile, &materialIndices[i], sizeof(unsigned int), &nBytesWrite, nullptr);
					}
					for (i = 0; i < ctrlPtCount; ++i)
					{
						for (j = 0; j < 4; ++j)
						{
							WriteFile(hFile, &ctrlIndices[i][j], sizeof(unsigned int), &nBytesWrite, nullptr);
						}
						for (j = 0; j < 4; ++j)
						{
							WriteFile(hFile, &ctrlBlendWeights[i][j], sizeof(double), &nBytesWrite, nullptr);
						}
					}
					for (i = 0; i < subsetCount; ++i)
					{
						WriteFile(hFile, &modelSubsets[i], sizeof(FBX_DATA::ModelSubset), &nBytesWrite, nullptr);
					}
				}
				return true;
			}
		};
	};
};