/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ISerialize.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <windows.h>

#define MAXTEXTLEN 128
namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			class ISerialize
			{
			private:
				char* m_FileInfoName;
			protected:
			public:
				ISerialize(const char* FileInfoName = "");
				virtual ~ISerialize();

				virtual bool SerializeIn(HANDLE hFile) = 0;
				virtual bool SerializeOut(HANDLE hFile) = 0;
			};
		};
	};
};