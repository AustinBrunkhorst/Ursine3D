/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
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
				mmeshCount(0), mmaterialCount(0), mboneCount(0),  maniCount(0), 
				mmeshlvlCount(0), mriglvlCount(0),
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
				maniNameVec.clear();
				mMeshLvVec.clear();
				mRigLvVec.clear();
			}

			bool ModelInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				unsigned int i = 0;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					char tmp_name[MAXTEXTLEN];
					ReadFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					name = tmp_name;
					ReadFile(hFile, &mmeshCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mmaterialCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mboneCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &maniCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mmeshlvlCount, sizeof(unsigned int), &nBytesRead, nullptr);
					ReadFile(hFile, &mriglvlCount, sizeof(unsigned int), &nBytesRead, nullptr);

					mMeshInfoVec.resize(mmeshCount);
					for (i = 0; i < mmeshCount; ++i)
						mMeshInfoVec[i].SerializeIn(hFile);
					mMtrlInfoVec.resize(mmaterialCount);
					for (i = 0; i < mmaterialCount; ++i)
						mMtrlInfoVec[i].SerializeIn(hFile);
					mBoneInfoVec.resize(mboneCount);
					for (i = 0; i < mboneCount; ++i)
						mBoneInfoVec[i].SerializeIn(hFile);
					maniNameVec.resize(maniCount);
					for (i = 0; i < maniCount; ++i)
					{
						ReadFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
						maniNameVec[i] = tmp_name;
					}
					mMeshLvVec.resize(mmeshlvlCount);
					for (i = 0; i < mmeshlvlCount; ++i)
						ReadFile(hFile, &mMeshLvVec[i], sizeof(MeshInLvl), &nBytesRead, nullptr);
					mRigLvVec.resize(mriglvlCount);
					for (i = 0; i < mriglvlCount; ++i)
						ReadFile(hFile, &mRigLvVec[i], sizeof(RigInLvl), &nBytesRead, nullptr);
				}
				return true;
			}

			bool ModelInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				unsigned int i = 0;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					char tmp_name[MAXTEXTLEN];
					lstrcpy(tmp_name, name.c_str());
					WriteFile(hFile, tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);					
					WriteFile(hFile, &mmeshCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mmaterialCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mboneCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &maniCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mmeshlvlCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &mriglvlCount, sizeof(unsigned int), &nBytesWrite, nullptr);

					if (mMeshInfoVec.size() > 0)
					{
						for (auto iter : mMeshInfoVec)
							iter.SerializeOut(hFile);
					}
					if (mMtrlInfoVec.size() > 0)
					{
						for (auto iter : mMtrlInfoVec)
							iter.SerializeOut(hFile);
					}
					if (mBoneInfoVec.size() > 0)
					{
						for (auto iter : mBoneInfoVec)
							iter.SerializeOut(hFile);
					}
					if (maniNameVec.size() > 0)
					{
						for (auto iter : maniNameVec)
						{
							lstrcpy(tmp_name, iter.c_str());
							WriteFile(hFile, &tmp_name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
						}
					}
					if (mMeshLvVec.size() > 0)
					{
						for (auto iter : mMeshLvVec)
							WriteFile(hFile, &iter, sizeof(MeshInLvl), &nBytesWrite, nullptr);
					}
					if (mRigLvVec.size() > 0)
					{
						for (auto iter : mRigLvVec)
							WriteFile(hFile, &iter, sizeof(RigInLvl), &nBytesWrite, nullptr);
					}
				}
				return true;
			}
		};
	};
};