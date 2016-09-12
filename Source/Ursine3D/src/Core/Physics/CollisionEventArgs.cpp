/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** CollisionEventArgs.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CollisionEventArgs.h"

namespace ursine
{
	namespace physics
	{
		Contact::Contact(void)
			: appliedImpulse( -123.4f )
			, penDistance( -123.4f )
		{
			
		}
	}
}
