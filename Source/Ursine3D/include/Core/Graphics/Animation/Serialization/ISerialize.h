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