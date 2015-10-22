#pragma once

#include "ISerialize.h"
#include "Utilities.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			class SkinInfo : public ISerialize
			{
			public:
				// every info might need name
				FBX_DATA::Joint		mbones;

				SkinInfo();
				virtual ~SkinInfo();

				void ReleaseData();
				virtual bool SerializeIn(HANDLE hFile);
				virtual bool SerializeOut(HANDLE hFile);
			};
		};
	};
};