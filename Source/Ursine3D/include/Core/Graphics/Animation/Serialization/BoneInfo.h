/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BoneInfo.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "ISerialize.h"
#include "AnimationDef.h"

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			class BoneInfo : public ISerialize
			{
			public:
				char name[MAXTEXTLEN];
				int mParentIndex;

				// bind - local coord system that the entire skin is defined relative to
				// local tm. local about to the skinned mesh
				pseudodx::XMFLOAT3 bindPosition;
				pseudodx::XMFLOAT4 bindRotation;
				pseudodx::XMFLOAT3 bindScaling;

				// bone space - the space that influences the vertices. so-called offset transformation
				// bone offset tm
				pseudodx::XMFLOAT3 boneSpacePosition;
				pseudodx::XMFLOAT4 boneSpaceRotation;
				pseudodx::XMFLOAT3 boneSpaceScaling;

				BoneInfo();
				virtual ~BoneInfo();

				void ReleaseData();
				virtual bool SerializeIn(HANDLE hFile);
				virtual bool SerializeOut(HANDLE hFile);
			};
		};
	};
};