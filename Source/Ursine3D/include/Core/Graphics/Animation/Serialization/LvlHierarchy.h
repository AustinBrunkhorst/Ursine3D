/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Utilities.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/
#pragma once
#include "SMat4.h"

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

				MeshInLvl() : mParentIndex(-1) {}
			};

			// This structure contains hierarchy among bones.
			struct RigInLvl
			{
				int mParentIndex;
				RigInLvl() : mParentIndex(-1) {}
			};
		}
	}
}