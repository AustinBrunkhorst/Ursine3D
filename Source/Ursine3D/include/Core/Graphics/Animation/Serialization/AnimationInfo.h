/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "BoneInfo.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			typedef std::vector<unsigned int> KeyIndex;
			typedef std::vector<KeyIndex> KeyIndices;
			typedef std::vector<FBX_DATA::KeyFrame> KFrame;
			typedef std::vector<KFrame> KFrames;
			typedef std::vector<KFrames> KFrms;
			// mesh of the model's animation data
			struct AnimData
			{
				std::string clipname;
				unsigned int clipCount;	// how many clips in this animation?
				unsigned int boneCount;	// how many bones in this model
				KeyIndices keyIndices;	// how many keys does each bone have?
				KFrms keyframes;

				AnimData() : clipname(""), clipCount(0), boneCount(0) {}
				void ReleaseData();
			};

			// model's animation data
			class AnimInfo : public ISerialize
			{
			public:
				std::string name;  // model name
				unsigned int animCount; // animation count is same as the number of meshes of the model
				std::vector<AnimData> animDataArr;	// and so does animData

				/** @brief animation information constructor
				*
				*  this will construct animation information object
				*
				*  @return nothing
				*/
				AnimInfo() : name(""), animCount(0), ISerialize("") {}

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