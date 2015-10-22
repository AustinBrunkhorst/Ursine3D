#pragma once

#include "MeshInfo.h"
#include "SkinInfo.h"
#include "AnimationInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			class ModelInfo : public ISerialize
			{
			public:
				char name[MAXTEXTLEN];

				// mesh data	 
				unsigned int	 mmeshCount;
				MeshInfo*		 marrMeshes;

				// skin data
				unsigned int	mskinCount;
				SkinInfo*		marrSkins;

				// anim data
				unsigned int	manimCount;
				AnimInfo*		marrAnims;

				ModelInfo();
				virtual ~ModelInfo();
				void ReleaseData();

				bool SerializeIn(HANDLE hFile);
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};