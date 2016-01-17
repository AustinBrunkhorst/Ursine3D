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
				std::vector<MeshInfo> mMeshInfoVec;

				// material data
				unsigned int	mmaterialCount;
				std::vector<MaterialInfo>	mMtrlInfoVec;

				// skin data
				unsigned int	mboneCount;
				std::vector<BoneInfo>	mBoneInfoVec;
				
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
			};
		};
	};
};