#pragma once

#include "UrsinePrecompiled.h"
#include "BoneInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			BoneInfo::BoneInfo()
				: ISerialize("")
			{
			}

			BoneInfo::~BoneInfo()
			{
				ReleaseData();
			}

			void BoneInfo::ReleaseData()
			{
			}

			bool BoneInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				if (hFile != INVALID_HANDLE_VALUE)
				{
					ReadFile(hFile, &name, sizeof(char) * MAXTEXTLEN, &nBytesRead, nullptr);
					ReadFile(hFile, &mParentIndex, sizeof(int), &nBytesRead, nullptr);
					ReadFile(hFile, &bindPosition, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
					ReadFile(hFile, &bindRotation, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
					ReadFile(hFile, &bindScaling, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
					ReadFile(hFile, &boneSpacePosition, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
					ReadFile(hFile, &boneSpaceRotation, sizeof(pseudodx::XMFLOAT4), &nBytesRead, nullptr);
					ReadFile(hFile, &boneSpaceScaling, sizeof(pseudodx::XMFLOAT3), &nBytesRead, nullptr);
				}
				return true;
			}

			bool BoneInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				if (hFile != INVALID_HANDLE_VALUE)
				{
					WriteFile(hFile, &name, sizeof(char) * MAXTEXTLEN, &nBytesWrite, nullptr);
					WriteFile(hFile, &mParentIndex, sizeof(int), &nBytesWrite, nullptr);
					WriteFile(hFile, &bindPosition, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
					WriteFile(hFile, &bindRotation, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
					WriteFile(hFile, &bindScaling, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
					WriteFile(hFile, &boneSpacePosition, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
					WriteFile(hFile, &boneSpaceRotation, sizeof(pseudodx::XMFLOAT4), &nBytesWrite, nullptr);
					WriteFile(hFile, &boneSpaceScaling, sizeof(pseudodx::XMFLOAT3), &nBytesWrite, nullptr);
				}
				return true;
			}
		};
	};
};