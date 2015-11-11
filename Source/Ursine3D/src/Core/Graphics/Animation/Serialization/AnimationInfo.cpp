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
				keyframes = new KeyFrame**[clipCount];
				for (i = 0; i < clipCount; ++i)
				{
					keyframes[i] = new KeyFrame*[boneCount];
					for (j = 0; j < boneCount; ++j)
					{
						keyframes[i][j] = new KeyFrame[keyIndices[i][j]];
						for (k = 0; k < keyIndices[i][j]; ++k)
						{
							KeyFrame* currKF = &keyframes[i][j][k];
							ReadFile(hFile, currKF, sizeof(KeyFrame), &nByteRead, nullptr);
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
							KeyFrame* currKF = &keyframes[i][j][k];
							WriteFile(hFile, currKF, sizeof(KeyFrame), &nBytesWrite, nullptr);
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
					KeyFrame* KeyFrame = keyframes[index][i];
					if (nullptr == KeyFrame)
						continue;

					//t is before the animation started, so just return the first key frame.
					if (timePos <= KeyFrame[0].time)
					{
						XMVECTOR S = XMLoadFloat3(&KeyFrame[0].scl);
						XMVECTOR P = XMLoadFloat3(&KeyFrame[0].trans);
						XMVECTOR Q = XMLoadFloat4(&KeyFrame[0].rot);
						XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
						toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
					} // t is after the animation ended, so just return the last key frame.
					else if (timePos >= KeyFrame[keyCount - 1].time)
					{
						XMVECTOR S = XMLoadFloat3(&KeyFrame[keyCount - 1].scl);
						XMVECTOR P = XMLoadFloat3(&KeyFrame[keyCount - 1].trans);
						XMVECTOR Q = XMLoadFloat4(&KeyFrame[keyCount - 1].rot);
						XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
						toParentTMs[i] = XMMatrixAffineTransformation(S, zero, Q, P);
					} // t is between two key frames, so interpolate.
					else
					{
						for (unsigned int j = 0; j < keyCount - 1; ++j)
						{
							if (timePos >= KeyFrame[j].time && timePos <= KeyFrame[j + 1].time)
							{
								float lerpPercent = ((float)timePos - KeyFrame[j].time) / (KeyFrame[j + 1].time - KeyFrame[j].time);
								XMVECTOR s0 = XMLoadFloat3(&KeyFrame[j].scl);
								XMVECTOR s1 = XMLoadFloat3(&KeyFrame[j + 1].scl);
								XMVECTOR p0 = XMLoadFloat3(&KeyFrame[j].trans);
								XMVECTOR p1 = XMLoadFloat3(&KeyFrame[j + 1].trans);
								XMVECTOR q0 = XMLoadFloat4(&KeyFrame[j].rot);
								XMVECTOR q1 = XMLoadFloat4(&KeyFrame[j + 1].rot);
								XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
								XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
								XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);
								XMVECTOR zero = XMVectorSet(0.f, 0.f, 0.f, 1.0f);
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