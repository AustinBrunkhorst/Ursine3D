/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelInfo.cpp
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
#include "LevelInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			LevelInfo::LevelInfo()
				: mmeshlvlCount(0), marrMeshlvls(nullptr),
				mriglvlCount(0), marrRiglvls(nullptr),
				mMeshHierarchy(nullptr), mRigHierarchy(nullptr),
				ISerialize("")
			{
			}

			LevelInfo::~LevelInfo()
			{
			}

			void LevelInfo::ReleaseData()
			{
				if (marrMeshlvls)
				{
					delete marrMeshlvls;
					marrMeshlvls = nullptr;
				}
				if (marrRiglvls)
				{
					delete marrRiglvls;
					marrRiglvls = nullptr;
				}
				if (mMeshHierarchy)
				{
					delete mMeshHierarchy;
					mMeshHierarchy = nullptr;
				}
				if (mRigHierarchy)
				{
					delete mRigHierarchy;
					mRigHierarchy = nullptr;
				}
			}
			
			bool LevelInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					ReadFile(hFile, &mmeshlvlCount, sizeof(unsigned int), &nBytesRead, nullptr);
					marrMeshlvls = new MeshInLvl[mmeshlvlCount];
					marrRiglvls = new RigInLvl[mriglvlCount];
					for (unsigned i = 0; i < mmeshlvlCount; ++i)
					{
						ReadFile(hFile, &marrMeshlvls[i], sizeof(MeshInLvl), &nBytesRead, nullptr);
						ReadFile(hFile, &mMeshHierarchy[i], sizeof(unsigned int), &nBytesRead, nullptr);
					}
					for (unsigned i = 0; i < mriglvlCount; ++i)
					{
						ReadFile(hFile, &marrRiglvls[i], sizeof(MeshInLvl), &nBytesRead, nullptr);
						ReadFile(hFile, &mRigHierarchy[i], sizeof(unsigned int), &nBytesRead, nullptr);
					}
				}
				return true;
			}

			bool LevelInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					WriteFile(hFile, &mmeshlvlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					for (unsigned i = 0; i < mmeshlvlCount; ++i)
					{
						WriteFile(hFile, &marrMeshlvls[i], sizeof(MeshInLvl), &nBytesWrite, nullptr);
						WriteFile(hFile, &mMeshHierarchy[i], sizeof(unsigned int), &nBytesWrite, nullptr);
					}
					for (unsigned i = 0; i < mriglvlCount; ++i)
					{
						WriteFile(hFile, &marrRiglvls[i], sizeof(MeshInLvl), &nBytesWrite, nullptr);
						WriteFile(hFile, &mRigHierarchy[i], sizeof(unsigned int), &nBytesWrite, nullptr);
					}
				}
				return true;
			}
		};
	};
};