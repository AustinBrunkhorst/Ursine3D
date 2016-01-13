/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelInfo.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <ISerialize.h>

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			struct MeshInLvl
			{
				char name[MAXTEXTLEN];
				SMat4 meshTM;
			};

			struct RigInLvl
			{
				char name[MAXTEXTLEN];
			};

			class LevelInfo : public ISerialize
			{
			public:
				unsigned int	mmeshlvlCount;
				unsigned int	mriglvlCount;
				MeshInLvl*		marrMeshlvls;
				RigInLvl*		marrRiglvls;
				int*			mMeshHierarchy;	// parent indices
				int*			mRigHierarchy;	// parent indices

				/** @brief level information constructor
				*
				*  this will construct level information object
				*
				*  @return nothing
				*/
				LevelInfo();

				/** @brief level information destructor
				*
				*  this will destroy level information object
				*
				*  @return nothing
				*/
				virtual ~LevelInfo();

				/** @brief level information release function
				*
				*  this will release memory of the level information
				*
				*  @return nothing
				*/
				void ReleaseData();

				/** @brief level information serialize in function
				*
				*  this will read level information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeIn(HANDLE hFile);

				/** @brief level information serialize out function
				*
				*  this will write level information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};