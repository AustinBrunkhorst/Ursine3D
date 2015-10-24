#include "UrsinePrecompiled.h"
#include "AnimationInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			AnimInfo::AnimInfo()
				: clipCount(0), boneCount(0), keyIndices(nullptr), keyframes(nullptr), ISerialize("")
			{
			}

			AnimInfo::~AnimInfo()
			{
				ReleaseData();
			}

			void AnimInfo::ReleaseData()
			{
				unsigned int i, j;
				for (i = 0; i < clipCount; ++i)
				{
					for (j = 0; j < boneCount; ++j)
					{
						// 3d pt
						if (keyframes)
							delete[] keyframes[i][j];
					}
					// 2d pt
					if (keyIndices)
						delete[] keyIndices[i];
					if (keyframes)
						delete[] keyframes[i];
				}
				// 1d pt
				if (keyIndices)
					delete[] keyIndices;
				if (keyframes)
					delete[] keyframes;
			}

			bool AnimInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nByteRead;
				unsigned int i = 0, j = 0, k = 0;

				// serializing counts
				ReadFile(hFile, &clipCount, sizeof(unsigned int), &nByteRead, nullptr);
				ReadFile(hFile, &boneCount, sizeof(unsigned int), &nByteRead, nullptr);
				keyIndices = new unsigned int*[clipCount];
				for (i = 0; i < clipCount; ++i)
				{
					keyIndices[i] = new unsigned int[boneCount];
					for (j = 0; j < boneCount; ++j)
					{
						ReadFile(hFile, &keyIndices[i][j], sizeof(unsigned int), &nByteRead, nullptr);
					}
				}

				// serializing datas
				keyframes = new FBX_DATA::KeyFrame**[clipCount];
				for (i = 0; i < clipCount; ++i)
				{
					keyframes[i] = new FBX_DATA::KeyFrame*[boneCount];
					for (j = 0; j < boneCount; ++j)
					{
						keyframes[i][j] = new FBX_DATA::KeyFrame[keyIndices[i][j]];
						for (k = 0; k < keyIndices[i][j]; ++k)
						{
							FBX_DATA::KeyFrame* currKF = &keyframes[i][j][k];
							ReadFile(hFile, currKF, sizeof(FBX_DATA::KeyFrame), &nByteRead, nullptr);
						}
					}
				}
				return true;
			}

			bool AnimInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				unsigned int i = 0, j = 0, k = 0;

				// serializing counts
				WriteFile(hFile, &clipCount, sizeof(unsigned int), &nBytesWrite, nullptr);
				WriteFile(hFile, &boneCount, sizeof(unsigned int), &nBytesWrite, nullptr);
				for (i = 0; i < clipCount; ++i)
				{
					for (j = 0; j < boneCount; ++j)
					{
						WriteFile(hFile, &keyIndices[i][j], sizeof(unsigned int), &nBytesWrite, nullptr);
					}
				}

				// serializing datas
				for (i = 0; i < clipCount; ++i)
				{
					for (j = 0; j < boneCount; ++j)
					{
						for (k = 0; k < keyIndices[i][j]; ++k)
						{
							FBX_DATA::KeyFrame* currKF = &keyframes[i][j][k];
							WriteFile(hFile, currKF, sizeof(FBX_DATA::KeyFrame), &nBytesWrite, nullptr);
						}
					}
				}
				return true;
			}
		};
	};
};