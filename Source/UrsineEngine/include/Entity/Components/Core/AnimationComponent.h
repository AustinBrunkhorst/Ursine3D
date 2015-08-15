/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationComponent.h
** - Animation component
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "Component.h"
#include "Animation.h"

namespace Ursine
{
    namespace ECS
    {
        class AnimationComponent : public Component
        {
            SerializerAllow(AnimationComponent, current);
            Animation *current;

        public:
            AnimationComponent(void);
            ~AnimationComponent(void);

            void OnInitialize(void) override;

            void Update(void);

            void Set(const std::string &name);

            // Pauses the animation
            void Pause(void);

            // Plays the animation
            void Play(void);

            Animation *GetCurrent(void);
        };
    }
}

////////////////////////////////////////////////////////////////////////////////
#endif // !ANIMATION_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef ANIMATION_REGISTERED
#define ANIMATION_REGISTERED
namespace Ursine
{
    namespace ECS
    {
        RegisterComponent(AnimationComponent);
        RegisterComponentData(AnimationComponent, current);
    }
}
#endif
#endif