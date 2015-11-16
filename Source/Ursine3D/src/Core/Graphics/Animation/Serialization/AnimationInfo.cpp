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

			void AnimInfo::Interpolate(int index, double timePos, std::vector<XMMATRIX>& toParentTMs)
			{
				for (size_t i = 0; i < boneCount; ++i)
				{
					// rot * trsnl is the correct order in row major
					// which clips which bone? need which clip index
					size_t keyCount = keyIndices[index][i];
					FBX_DATA::KeyFrame* KeyFrame = keyframes[index][i];
					if (nullptr == KeyFrame)
						continue;

					//t is before the animation started, so just return the first key frame.
					if (timePos <= KeyFrame[0].time)
					{
						XMVECTOR S = XMVectorSet(1.f, 1.f, 1.f, 1.f);
						XMVECTOR P = Utilities::SetFloat4ToXMVector(KeyFrame[0].trans);
						XMVECTOR Q = Utilities::SetFloat4ToXMVector(KeyFrame[0].rot);
						XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
						XMMATRIX trnsl = XMMatrixTranslationFromVector(P);
						XMMATRIX rot = XMMatrixRotationQuaternion(Q);
						toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
					}
					// t is after the animation ended, so just return the last key frame.
					else if (timePos >= KeyFrame[keyCount - 1].time)
					{
						XMVECTOR S = XMVectorSet(1.f, 1.f, 1.f, 1.f);
						XMVECTOR P = Utilities::SetFloat4ToXMVector(KeyFrame[keyCount - 1].trans);
						XMVECTOR Q = Utilities::SetFloat4ToXMVector(KeyFrame[keyCount - 1].rot);
						XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
						XMMATRIX trnsl = XMMatrixTranslationFromVector(P);
						XMMATRIX rot = XMMatrixRotationQuaternion(Q);
						toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
					}
					// t is between two key frames, so interpolate.
					else
					{
						for (unsigned int j = 0; j < keyCount - 1; ++j)
						{
							if (timePos >= KeyFrame[j].time && timePos <= KeyFrame[j + 1].time)
							{
								float lerpPercent = ((float)timePos - KeyFrame[j].time) / (KeyFrame[j + 1].time - KeyFrame[j].time);
								XMVECTOR p0 = Utilities::SetFloat4ToXMVector(KeyFrame[j].trans);
								XMVECTOR p1 = Utilities::SetFloat4ToXMVector(KeyFrame[j + 1].trans);
								XMVECTOR q0 = Utilities::SetFloat4ToXMVector(KeyFrame[j].rot);
								XMVECTOR q1 = Utilities::SetFloat4ToXMVector(KeyFrame[j + 1].rot);
								XMVECTOR S = XMVectorSet(1.f, 1.f, 1.f, 1.f);
								XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
								XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);
								XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
								XMMATRIX trnsl = XMMatrixTranslationFromVector(P);
								XMMATRIX rot = XMMatrixRotationQuaternion(Q);
								toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
								break;
							}
						}
					}
				}
			}
		};
	};
};