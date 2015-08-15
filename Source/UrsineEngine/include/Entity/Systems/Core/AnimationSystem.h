/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationSystem.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#include "FilterSystem.h"
#include "SystemRegistrar.h"

namespace Ursine
{
    namespace ECS
    {
        class AnimationSystem : public FilterSystem
        {
        public:
            AnimationSystem(World *world);
            ~AnimationSystem();

            void OnInitialize(void) override;
            void Process(Entity *entity) override;
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !ZONE_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_SYSTEMS

#ifndef ANIMATION_SYSTEM_REGISTERED
#define ANIMATION_SYSTEM_REGISTERED

namespace Ursine
{
    namespace ECS
    {
        RegisterEntitySystem(AnimationSystem)
    }
}

#endif

#endif