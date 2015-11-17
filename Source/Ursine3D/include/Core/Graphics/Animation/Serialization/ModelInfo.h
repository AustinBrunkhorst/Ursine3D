#pragma once

#include "MeshInfo.h"
#include "MaterialInfo.h"
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

				// material data
				unsigned int	mmaterialCount;
				MaterialInfo*	marrMaterials;

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
				AnimInfo* FindAnimClip(int* index, const std::string& clipName) const;
			};
		};
	};
};