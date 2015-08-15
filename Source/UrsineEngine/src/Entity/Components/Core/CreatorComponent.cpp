/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CreatorComponent.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "CreatorComponent.h"
#include "Entity.h"

namespace Ursine
{
    namespace ECS
    {
        CreatorComponent::CreatorComponent(void)
            : BaseComponent()
        {

        }

        CreatorComponent::CreatorComponent(const Entity *creator)
            : BaseComponent()
            , _creator(creator->GetUniqueID())
        {

        }

        CreatorComponent::~CreatorComponent(void)
        {

        }

        Entity *CreatorComponent::Get(void)
        {
            return GetOwner()->GetWorld()->GetEntityUnique(_creator);
        }

        void CreatorComponent::Set(const Entity *creator)
        {
            _creator = creator->GetUniqueID();
        }
    }
}
