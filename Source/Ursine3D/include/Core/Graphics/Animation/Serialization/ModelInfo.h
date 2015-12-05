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

				/** @brief model information constructor
				*
				*  this will construct model information object
				*
				*  @return nothing
				*/
				ModelInfo();
				/** @brief model information destructor
				*
				*  this will destroy model information object
				*
				*  @return nothing
				*/
				virtual ~ModelInfo();
				/** @brief model information release function
				*
				*  this will release memory of the model information
				*
				*  @return nothing
				*/
				void ReleaseData();

				/** @brief model information serialize in function
				*
				*  this will read model information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeIn(HANDLE hFile);
				/** @brief model information serialize out function
				*
				*  this will write model information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeOut(HANDLE hFile);
				/** @brief find animation clip function
				*
				*  this will find animation clip by the name of the clip
				*  and store the index of the clip
				*
				*  @param index the pointer of the animation information
				*  @param clipName the name of the animation clip
				*  @return Animation Info of the animation clip
				*/
				AnimInfo* FindAnimClip(int* index, const std::string& clipName) const;
			};
		};
	};
};