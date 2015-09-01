/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationSystem.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimationSystem.h"
#include "AnimationComponent.h"
#include "RenderableComponent.h"

#include "Animation.h"

namespace ursine
{
    namespace ecs
    {
        AnimationSystem::AnimationSystem(World *world)
            : FilterSystem(world, Filter().All<AnimationComponent, Renderable>())
        {
            
        }

        AnimationSystem::~AnimationSystem(void)
        {
            
        }

        void AnimationSystem::OnInitialize(void)
        {
            FilterSystem::OnInitialize();
        }

        void AnimationSystem::Process(Entity *entity)
        {
            auto animation = entity->GetComponent<AnimationComponent>();

            animation->Update();

            auto current = animation->GetCurrent();

            if (current->FrameChanged())
            {
                auto renderable = entity->GetComponent<Renderable>();

                renderable->SetUVOffset(current->GetUVOffset());
                renderable->SetUVScale(current->GetUVScale());
            }
        }
    }
}