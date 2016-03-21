/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "EntitySystem.h"

namespace ursine
{
    class AnimationRig;

    namespace ecs
    {
        class Animator;

        class AnimatorSystem : public EntitySystem
        {
            ENTITY_SYSTEM;

        public:

            Meta(Enable)
            AnimatorSystem(World *world);

        private:

            void OnInitialize(void) override;
            void OnRemove(void) override;

            void onComponentAdded(EVENT_HANDLER(World));
            void onComponentRemoved(EVENT_HANDLER(World));

            void onUpdate(EVENT_HANDLER(World));

        #if defined(URSINE_WITH_EDITOR)

            void onEditorUpdate(EVENT_HANDLER(World));

        #endif

            void updateAnimator(Animator *animator, float dt);

            void debugDraw(Animator *animator, const AnimationRig *rig, const std::vector<SMat4> &vec);

            std::vector<Animator*> m_animators;
        } Meta(
            Enable,
            WhiteListMethods,
            AutoAddEntitySystem
        );
    }
}
