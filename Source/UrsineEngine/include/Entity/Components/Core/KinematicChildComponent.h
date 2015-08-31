/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationComponent.h
** - Kinematic child component
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef KINEMATIC_CHILD_COMPONENT_H
#define KINEMATIC_CHILD_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "Component.h"

namespace Ursine
{
    namespace ECS
    {
        class KinematicChild : public Component
        {
            friend class KinematicChildSystem;

            // The last frames position
            SVec3 _last_position;

        public:
            KinematicChild(void);
            ~KinematicChild(void);

            void OnInitialize(void) override;

			ALLOW_ALIGNED_ALLOC(16)
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !KINEMATIC_CHILD_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef KINEMATIC_CHILD_REGISTERED
#define KINEMATIC_CHILD_REGISTERED
namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(KinematicChild);
        //RegisterComponentData(AnimationComponent, current);
    }
}
#endif
#endif