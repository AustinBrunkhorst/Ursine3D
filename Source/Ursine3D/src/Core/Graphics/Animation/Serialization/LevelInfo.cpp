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
                    marrMeshlvls = new MeshInLvl[ mmeshlvlCount ];
                    for ( unsigned i = 0; i < mmeshlvlCount; ++i )
                        ReadFile(hFile, &marrMeshlvls[ i ], sizeof(MeshInLvl), &nBytesRead, nullptr);
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