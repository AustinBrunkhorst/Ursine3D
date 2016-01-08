/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationInfo.cpp
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "AnimationInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			void AnimData::ReleaseData()
			{
				unsigned int i, j;
				for (i = 0; i < clipCount; ++i)
				{
					for (j = 0; j < boneCount; ++j)
					{
						// 3d pt
						delete[] keyframes[i][j];
					}
					// 2d pt
					delete[] keyIndices[i];
					delete[] keyframes[i];
				}
				// 1d pt
				delete[] keyIndices;
				delete[] keyframes;
				keyIndices = nullptr;
				keyframes = nullptr;
			}

			AnimInfo::~AnimInfo()
			{
			}

			void AnimInfo::ReleaseData()
			{
				if (nullptr == animDataArr)
					return;

				for (unsigned int i = 0; i < animCount; ++i)
					animDataArr[i].ReleaseData();
				delete[] animDataArr;
				animDataArr = nullptr;
			}

			bool AnimInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nByteRead;
				unsigned int i = 0, j = 0, k = 0, l = 0;

				ReadFile(hFile, &name, sizeof(char) * MAXTEXTLEN, &nByteRead, nullptr);
				ReadFile(hFile, &animCount, sizeof(unsigned int), &nByteRead, nullptr);
				for (i = 0; i < animCount; ++i)
				{
					// serializing counts
					ReadFile(hFile, &animDataArr[i].clipname, sizeof(char) * MAXTEXTLEN, &nByteRead, nullptr);
					ReadFile(hFile, &animDataArr[i].clipCount, sizeof(unsigned int), &nByteRead, nullptr);
					ReadFile(hFile, &animDataArr[i].boneCount, sizeof(unsigned int), &nByteRead, nullptr);
					animDataArr[i].keyIndices = new unsigned int*[animDataArr[i].clipCount];
					animDataArr[i].keyframes = new FBX_DATA::KeyFrame**[animDataArr[i].clipCount];
					for (j = 0; j < animDataArr[i].clipCount; ++j)
					{
						animDataArr[i].keyIndices[j] = new unsigned int[animDataArr[i].boneCount];
						animDataArr[i].keyframes[j] = new FBX_DATA::KeyFrame*[animDataArr[i].boneCount];
						for (k = 0; k < animDataArr[i].boneCount; ++k)
						{
							ReadFile(hFile, &animDataArr[i].keyIndices[j][k], sizeof(unsigned int), &nByteRead, nullptr);
							animDataArr[i].keyframes[j][k] = new FBX_DATA::KeyFrame[animDataArr[i].keyIndices[j][k]];
							for (l = 0; l <animDataArr[i].keyIndices[j][k]; ++l)
							{
								FBX_DATA::KeyFrame* currKF = &animDataArr[i].keyframes[j][k][l];
								ReadFile(hFile, currKF, sizeof(FBX_DATA::KeyFrame), &nByteRead, nullptr);
							}
						}
					}
				}
				return true;
			}

			bool AnimInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				unsigned int i = 0, j = 0, k = 0, l = 0;

				WriteFile(hFile, &name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
				WriteFile(hFile, &animCount, sizeof(unsigned int), &nBytesWrite, nullptr);
				for (i = 0; i < animCount; ++i)
				{
					// serializing counts
					WriteFile(hFile, &animDataArr[i].clipname, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					WriteFile(hFile, &animDataArr[i].clipCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					WriteFile(hFile, &animDataArr[i].boneCount, sizeof(unsigned int), &nBytesWrite, nullptr);
					for (j = 0; j < animDataArr[i].clipCount; ++j)
					{
						for (k = 0; k < animDataArr[i].boneCount; ++k)
						{
							WriteFile(hFile, &animDataArr[i].keyIndices[j][k], sizeof(unsigned int), &nBytesWrite, nullptr);
							for (l = 0; l <animDataArr[i].keyIndices[j][k]; ++l)
							{
								FBX_DATA::KeyFrame* currKF = &animDataArr[i].keyframes[j][k][l];
								WriteFile(hFile, currKF, sizeof(FBX_DATA::KeyFrame), &nBytesWrite, nullptr);
							}
						}
					}
				}
				return true;
			}
		};
	};
};