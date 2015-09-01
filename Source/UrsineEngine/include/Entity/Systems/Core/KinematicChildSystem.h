/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** KinematicChildSystem.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef KINEMATIC_CHILD_SYSTEM_H
#define KINEMATIC_CHILD_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#include "FilterSystem.h"

namespace ursine
{
    namespace ecs
    {
        class KinematicChildSystem : public FilterSystem
        {
            // the stored delta time
            float _dt;

        public:
            KinematicChildSystem(World *world);
            ~KinematicChildSystem(void);

            void Begin(void) override;
            void Process(Entity *entity) override;
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !RENDER_SYSTEM_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_SYSTEMS
#ifndef KINEMATIC_CHILD_SYSTEM_REGISTERED
#define KINEMATIC_CHILD_SYSTEM_REGISTERED

namespace ursine
{
    namespace ecs
    {
        RegisterEntitySystem(KinematicChildSystem);
    }
}

#endif
#endif
