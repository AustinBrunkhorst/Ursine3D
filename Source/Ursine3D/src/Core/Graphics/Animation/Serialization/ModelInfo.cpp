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
				mmeshCount(0), mmaterialCount(0), mboneCount(0), 
				ISerialize("")
			{
			}

			ModelInfo::~ModelInfo()
			{
				ReleaseData();
			}

			void ModelInfo::ReleaseData()
			{
				mMeshInfoVec.clear();
				mMtrlInfoVec.clear();
				mBoneInfoVec.clear();
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

					mMeshInfoVec.resize(mmeshCount);
					for (i = 0; i < mmeshCount; ++i)
						mMeshInfoVec[i].SerializeIn(hFile);
					mMtrlInfoVec.resize(mmaterialCount);
					for (i = 0; i < mmaterialCount; ++i)
						mMtrlInfoVec[i].SerializeIn(hFile);
					mBoneInfoVec.resize(mboneCount);
					for (i = 0; i < mboneCount; ++i)
						mBoneInfoVec[i].SerializeIn(hFile);
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

					for (auto iter : mMeshInfoVec)
						iter.SerializeOut(hFile);
					for (auto iter : mMtrlInfoVec)
						iter.SerializeOut(hFile);
					for (auto iter : mBoneInfoVec)
						iter.SerializeOut(hFile);
				}
				return true;
			}
		};
	};
};