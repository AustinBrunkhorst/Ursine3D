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
			// This structure contains hierarchy among meshes. This will makes easy to find it.
			struct MeshInLvl
			{
				SMat4 meshTM;
				int mParentIndex;
			};

			// This structure contains hierarchy among bones.
			struct RigInLvl
			{
				int mParentIndex;
			};

			class ModelInfo : public ISerialize
			{
			public:
				std::string name;

				// mesh data	 
				unsigned int	mmeshCount;
				std::vector<MeshInfo> mMeshInfoVec;

				// material data
				unsigned int	mmaterialCount;
				std::vector<MaterialInfo> mMtrlInfoVec;

				// skin data
				unsigned int	mboneCount;
				std::vector<BoneInfo> mBoneInfoVec;

				// animation data - this will store the list of animation which will be used to 
				// save/load the result of editor
				// so that jdl format can load animations by this list
				// the name we will store, is jani's name
				// if loading fbx, maniCount = 0, maniNameVec = empty.
				// after modifying jdl and store it, then there will be some changes
				unsigned int maniCount;
				std::vector< std::string > maniNameVec;

				// level info - hierarchy
				unsigned int	mmeshlvlCount;
				unsigned int	mriglvlCount;
				std::vector<MeshInLvl> mMeshLvVec;
				std::vector<RigInLvl> mRigLvVec;	
				
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