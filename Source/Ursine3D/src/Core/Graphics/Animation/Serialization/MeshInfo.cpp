/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MeshInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

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
				meshVtxInfoCount(0), mtrlCount(0), mtrlIndexCount(0),
				meshVtxInfos(nullptr), materialIndices(nullptr),
				ISerialize("")
			{
				*mtrlName = { nullptr };
			}

			MeshInfo::~MeshInfo()
			{
				ReleaseData();
			}

			void MeshInfo::ReleaseData()
			{
				if (meshVtxInfos)
				{
					delete[] meshVtxInfos;
					meshVtxInfos = nullptr;
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
			}

			bool MeshInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				unsigned int i = 0;

				if (hFile != INVALID_HANDLE_VALUE)
				{
					ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					ReadFile(hFile, &meshVtxInfoCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesRead, nullptr);

					meshVtxInfos = new MeshVertex[meshVtxInfoCount];
					for (i = 0; i < meshVtxInfoCount; ++i)
					{
						ReadFile(hFile, &meshVtxInfos[i], sizeof(MeshVertex), &nBytesRead, nullptr);
					}
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
					WriteFile(hFile, &meshVtxInfoCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesWrite, nullptr);

					for (i = 0; i < meshVtxInfoCount; ++i)
					{
						WriteFile(hFile, &meshVtxInfos[i], sizeof(MeshVertex), &nBytesWrite, nullptr);
					}
					for (i = 0; i < mtrlCount; ++i)
					{
						WriteFile(hFile, mtrlName[i], sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					}
					for (i = 0; i < mtrlIndexCount; ++i)
					{
						WriteFile(hFile, &materialIndices[i], sizeof(unsigned int), &nBytesWrite, nullptr);
					}
				}
				return true;
			}
		};
	};
};