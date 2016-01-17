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
				meshVtxInfoCount(0), meshVtxIdxCount(0), mtrlCount(0), mtrlIndexCount(0), ISerialize("")
			{
			}

			MeshInfo::~MeshInfo()
			{
				ReleaseData();
			}

			void MeshInfo::ReleaseData()
			{
				mtrlName.clear();
				meshVtxInfos.clear();
				meshVtxIndices.clear();
				materialIndices.clear();
			}

			bool MeshInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				unsigned int i = 0;

				if (hFile != INVALID_HANDLE_VALUE)
				{
					ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					ReadFile(hFile, &meshVtxInfoCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &meshVtxIdxCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesRead, nullptr);


					for (i = 0; i < meshVtxInfoCount; ++i)
					{
						MeshVertex newMV;
						ReadFile(hFile, &newMV, sizeof(MeshVertex), &nBytesRead, nullptr);
						meshVtxInfos.push_back(newMV);
					}
					for (i = 0; i < meshVtxIdxCount; ++i)
					{
						unsigned int mvi;
						ReadFile(hFile, &mvi, sizeof(unsigned int), &nBytesRead, nullptr);
						meshVtxIndices.push_back(mvi);
					}
					for (i = 0; i < mtrlCount; ++i)
					{
						char name[MAXTEXTLEN];
						ReadFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
						mtrlName.push_back(name);
					}
					for (i = 0; i < mtrlIndexCount; ++i)
					{
						unsigned int mi;
						ReadFile(hFile, &mi, sizeof(unsigned int), &nBytesRead, nullptr);
						materialIndices.push_back(mi);
					}
				}
				return true;
			}

			bool MeshInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				unsigned int i = 0;

				if (hFile != INVALID_HANDLE_VALUE)
				{
					WriteFile(hFile, name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					WriteFile(hFile, &meshVtxInfoCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &meshVtxIdxCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mtrlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mtrlIndexCount, sizeof(unsigned int), &nBytesWrite, nullptr);

					for (i = 0; i < meshVtxInfoCount; ++i)
						WriteFile(hFile, &meshVtxInfos[i], sizeof(MeshVertex), &nBytesWrite, nullptr);
					for (i = 0; i < meshVtxIdxCount; ++i)
						WriteFile(hFile, &meshVtxIndices[i], sizeof(unsigned int), &nBytesWrite, nullptr);
					for (i = 0; i < mtrlCount; ++i)
						WriteFile(hFile, mtrlName[i].c_str(), sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					for (i = 0; i < mtrlIndexCount; ++i)
						WriteFile(hFile, &materialIndices[i], sizeof(unsigned int), &nBytesWrite, nullptr);
				}
				return true;
			}
		};
	};
};