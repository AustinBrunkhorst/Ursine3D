/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TutorialResourcesComponent.h
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

class TutorialResources : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    TutorialResources(void);

    std::string worldToMerge;

} Meta(Enable);
