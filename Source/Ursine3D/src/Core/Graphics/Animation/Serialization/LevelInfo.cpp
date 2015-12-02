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
				: mmeshlvlCount(0), marrMeshlvls(0), ISerialize("")
			{
			}

			LevelInfo::~LevelInfo()
			{
			}
			
			void LevelInfo::ReleaseData()
			{
				if (marrMeshlvls)
				{
					delete[] marrMeshlvls;
					marrMeshlvls = nullptr;
				}
			}
			
			bool LevelInfo::SerializeIn(HANDLE hFile)
			{
				DWORD nBytesRead;
				if (INVALID_HANDLE_VALUE != hFile)
				{
					ReadFile(hFile, &mmeshlvlCount, sizeof(unsigned int), &nBytesRead, nullptr);
					for (unsigned i = 0; i < mmeshlvlCount; ++i)
						ReadFile(hFile, &marrMeshlvls[i], sizeof(MeshInLvl), &nBytesRead, nullptr);
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
						WriteFile(hFile, &marrMeshlvls[i], sizeof(MeshInLvl), &nBytesWrite, nullptr);
				}
				return true;
			}
		};
	};
};