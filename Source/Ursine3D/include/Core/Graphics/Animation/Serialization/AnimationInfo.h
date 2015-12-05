/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationInfo.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AnimationDef.h"
#include "ISerialize.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			//storing AnimationData
			class AnimInfo : public ISerialize
			{
			public:
				char name[MAXTEXTLEN];
				unsigned int clipCount; // how many clips in this animation?
				unsigned int boneCount; // how many bones in this model?
				unsigned int** keyIndices; // how many keys does each bone have?
				FBX_DATA::KeyFrame*** keyframes;

				/** @brief animation information constructor
				*
				*  this will construct animation information object
				*
				*  @return nothing
				*/
				AnimInfo();
				/** @brief animation information destructor
				*
				*  this will destroy animation information object
				*
				*  @return nothing
				*/
				virtual ~AnimInfo();
				/** @brief animation information release function
				*
				*  this will release memory of the animation information
				*
				*  @return nothing
				*/
				void ReleaseData();

				/** @brief animation information serialize in function
				*
				*  this will read animation information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeIn(HANDLE hFile);
				/** @brief animation information serialize out function
				*
				*  this will write animation information
				*
				*  @param handle of the file
				*  @return if succeed return true, else return false
				*/
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};