#pragma once

#include <string>
#include "UrsinePrecompiled.h"
#include "LevelInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			LevelInfo::LevelInfo()
				: ISerialize("")
			{
			}

			LevelInfo::~LevelInfo()
			{
			}
			
			bool LevelInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				unsigned int i = 0;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					ReadFile(hFile, &mmeshCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mmaterialCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mboneCount, sizeof(unsigned int), &nBytesRead, nullptr);
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
					marrBones = new BoneInfo[mboneCount];
					for (i = 0; i < mboneCount; ++i)
					{
						marrBones[i].SerializeIn(hFile);
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
					WriteFile(hFile, &mboneCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &manimCount, sizeof(unsigned int), &nBytesWrite, nullptr);

					for (i = 0; i < mmeshCount; ++i)
					{
						marrMeshes[i].SerializeOut(hFile);
					}
					for (i = 0; i < mmaterialCount; ++i)
					{
						marrMaterials[i].SerializeOut(hFile);
					}
					for (i = 0; i < mboneCount; ++i)
					{
						marrBones[i].SerializeOut(hFile);
					}
					for (i = 0; i < manimCount; ++i)
					{
						marrAnims[i].SerializeOut(hFile);
					}
				}
				return true;
			}

			AnimInfo* ModelInfo::FindAnimClip(int* index, const std::string& clipName) const
			{
				for (unsigned int i = 0; i < manimCount; ++i)
				{
					if (clipName == marrAnims[i].name)
					{
						*index = i;
						return &marrAnims[i];
					}
				}
				return nullptr;
			}
		};
	};
};