/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SelectedComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class DisableSelection : public ursine::ecs::Component
{
	NATIVE_COMPONENT;
public:
	Meta(Enable)
	DisableSelection(void);

} Meta(Enable, WhiteListMethods, /*HiddenInInspector*/);
