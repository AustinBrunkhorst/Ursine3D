/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CreatorComponent.h
** - Stores the creator of an entity
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef CREATOR_COMPONENT_H
#define CREATOR_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "EntityConfig.h"

namespace Ursine
{
    namespace ECS
    {
        class CreatorComponent : public Component
        {
            EntityUniqueID _creator;

        public:
            CreatorComponent(void);
            CreatorComponent(const Entity *creator);

            ~CreatorComponent(void);

            Entity *Get(void);
            void Set(const Entity *creator);
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !CREATOR_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef CREATOR_COMPONENT_REGISTERED
#define CREATOR_COMPONENT_REGISTERED

namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(CreatorComponent);
    }
}

#endif // REGISTER_COMPONENTS
#endif // !CREATOR_COMPONENT_REGISTERED