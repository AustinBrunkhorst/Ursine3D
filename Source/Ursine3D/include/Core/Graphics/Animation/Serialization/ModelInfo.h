/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModelInfo.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "MeshInfo.h"
#include "MaterialInfo.h"
#include "BoneInfo.h"
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
				unsigned int	mboneCount;
				BoneInfo*		marrBones;

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