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

				AnimInfo();
				virtual ~AnimInfo();
				void ReleaseData();

				bool SerializeIn(HANDLE hFile);
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};