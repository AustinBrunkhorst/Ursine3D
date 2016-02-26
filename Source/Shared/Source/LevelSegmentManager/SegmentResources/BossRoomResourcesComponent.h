/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BossRoomResourcesComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <Array.h>

class BossRoomResources : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    BossRoomResources(void);

    ursine::Array<std::string> archetypesToLoad;

} Meta(Enable);