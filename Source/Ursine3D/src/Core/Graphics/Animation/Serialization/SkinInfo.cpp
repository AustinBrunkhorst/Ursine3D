#pragma once

#include "UrsinePrecompiled.h"
#include "SkinInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			SkinInfo::SkinInfo()
				: ISerialize("")
			{
			}

			SkinInfo::~SkinInfo()
			{
				ReleaseData();
			}

			void SkinInfo::ReleaseData()
			{
			}

			bool SkinInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				if (hFile != INVALID_HANDLE_VALUE)
				{
					ReadFile(hFile, &mbones, sizeof(FBX_DATA::Joint), &nBytesRead, nullptr);
				}
				return true;
			}

			bool SkinInfo::SerializeOut(HANDLE hFile)
			{
				DWORD nBytesWrite;
				if (hFile != INVALID_HANDLE_VALUE)
				{
					WriteFile(hFile, &mbones, sizeof(FBX_DATA::Joint), &nBytesWrite, nullptr);
				}
				return true;
			}
		};
	};
};