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
				: mmeshlvlCount(0), mriglvlCount(0), ISerialize("")
			{
			}

			LevelInfo::~LevelInfo()
			{
				ReleaseData();
			}

			void LevelInfo::ReleaseData()
			{
				mMeshLvVec.clear();
				mRigLvVec.clear();
			}
			
			bool LevelInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					char tmp_name[MAXTEXTLEN];
					ReadFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					name = tmp_name;
					ReadFile(hFile, &mmeshlvlCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mriglvlCount, sizeof(unsigned int), &nBytesRead, nullptr);

					MeshInLvl newMl;
					RigInLvl newRl;
					for (unsigned i = 0; i < mmeshlvlCount; ++i)
						ReadFile(hFile, &newMl, sizeof(MeshInLvl), &nBytesRead, nullptr);
					for (unsigned i = 0; i < mriglvlCount; ++i)
						ReadFile(hFile, &newRl, sizeof(RigInLvl), &nBytesRead, nullptr);

					mMeshLvVec.push_back(newMl);
					mRigLvVec.push_back(newRl);
				}
				return true;
			}

			bool LevelInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					char tmp_name[MAXTEXTLEN];
					lstrcpy(tmp_name, name.c_str());
					WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					WriteFile(hFile, &mmeshlvlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mriglvlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					for (unsigned i = 0; i < mmeshlvlCount; ++i)
						WriteFile(hFile, &mMeshLvVec[i], sizeof(MeshInLvl), &nBytesWrite, nullptr);
					for (unsigned i = 0; i < mriglvlCount; ++i)
						WriteFile(hFile, &mRigLvVec[i], sizeof(RigInLvl), &nBytesWrite, nullptr);
				}
				return true;
			}
		};
	};
};