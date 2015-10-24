#pragma once

#include <string>
#include "UrsinePrecompiled.h"
#include "ModelInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			ModelInfo::ModelInfo()
				:
				mmeshCount(0), marrMeshes(nullptr),
				mmaterialCount(0), marrMaterials(nullptr),
				mskinCount(0), marrSkins(nullptr),
				manimCount(0), marrAnims(nullptr),
				ISerialize("")
			{
			}

			ModelInfo::~ModelInfo()
			{
			}

			void ModelInfo::ReleaseData()
			{
				if (marrMeshes)
				{
					delete[] marrMeshes;
					marrMeshes = nullptr;
				}
				if (marrMaterials)
				{
					delete[] marrMaterials;
					marrMaterials = nullptr;
				}
				if (marrSkins)
				{
					delete[] marrSkins;
					marrSkins = nullptr;
				}
				if (marrAnims)
				{
					delete[] marrAnims;
					marrAnims = nullptr;
				}
			}

			bool ModelInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				unsigned int i = 0;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					ReadFile(hFile, &mmeshCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mmaterialCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mskinCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &manimCount, sizeof(unsigned int), &nBytesRead, nullptr);

					marrMeshes = new MeshInfo[mmeshCount];
					for (i = 0; i < mmeshCount; ++i)
					{
						marrMeshes[i].SerializeIn(hFile);
					}
					marrMaterials = new MaterialInfo[mmaterialCount];
					for (i = 0; i < mmaterialCount; ++i)
					{
						marrMaterials[i].SerializeIn(hFile);
					}
					marrSkins = new SkinInfo[mskinCount];
					for (i = 0; i < mskinCount; ++i)
					{
						marrSkins[i].SerializeIn(hFile);
					}
					marrAnims = new AnimInfo[manimCount];
					for (i = 0; i < manimCount; ++i)
					{
						marrAnims[i].SerializeIn(hFile);
					}
				}
				return true;
			}

			bool ModelInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				unsigned int i = 0;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					WriteFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					WriteFile(hFile, &mmeshCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mmaterialCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mskinCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &manimCount, sizeof(unsigned int), &nBytesWrite, nullptr);

					for (i = 0; i < mmeshCount; ++i)
					{
						marrMeshes[i].SerializeOut(hFile);
					}
					for (i = 0; i < mmaterialCount; ++i)
					{
						marrMaterials[i].SerializeOut(hFile);
					}
					for (i = 0; i < mskinCount; ++i)
					{
						marrSkins[i].SerializeOut(hFile);
					}
					for (i = 0; i < manimCount; ++i)
					{
						marrAnims[i].SerializeOut(hFile);
					}
				}
				return true;
			}
		};
	};
};