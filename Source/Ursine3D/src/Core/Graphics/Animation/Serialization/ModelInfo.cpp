/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

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
				mboneCount(0), marrBones(nullptr),
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
				if (marrBones)
				{
					delete[] marrBones;
					marrBones = nullptr;
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
					ReadFile(hFile, &mboneCount, sizeof(unsigned int), &nBytesRead, nullptr);

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
				}
				return true;
			}
		};
	};
};