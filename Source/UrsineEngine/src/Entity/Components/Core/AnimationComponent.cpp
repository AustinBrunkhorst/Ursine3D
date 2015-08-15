/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationComponent.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimationComponent.h"
#include "RenderableComponent.h"

#include "ContentManager.h"

#include "Texture2D.h"

namespace Ursine
{
    namespace ECS
    {
        AnimationComponent::AnimationComponent(void)
            : BaseComponent()
        {
            
        }

        AnimationComponent::~AnimationComponent(void)
        {
            
        }

        void AnimationComponent::OnInitialize(void)
        {
            if (current)
            {
                auto renderable = GetOwner()->GetComponent<Renderable>();

                renderable->SetTexture(current->GetTexture());

                renderable->SetUVOffset(current->GetUVOffset());
                renderable->SetUVScale(current->GetUVScale());
            }
        }

        void AnimationComponent::Update(void)
        {
            current->Update();
        }

        void AnimationComponent::Set(const std::string &name)
        {
            // the same
            if (current->GetPath() == name)
                return;

            auto old_id = current->GetTexture()->GetID();

            current = gContentManager->Load<Animation>(name);

            auto texture = current->GetTexture();

            auto renderable = GetOwner()->GetComponent<Renderable>();

            // texture changed?
            if (old_id != texture->GetID())
                renderable->SetTexture(texture);

            renderable->SetUVOffset(current->GetUVOffset());
            renderable->SetUVScale(current->GetUVScale());
        }

        void AnimationComponent::Pause(void)
        {
            current->Pause();
        }

        // Plays the animation
        void AnimationComponent::Play(void)
        {
            current->Play();
        }

        Animation* AnimationComponent::GetCurrent(void)
        {
            return current;
        }
    }
}
