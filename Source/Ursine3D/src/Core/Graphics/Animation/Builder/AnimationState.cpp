/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationState.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "AnimationState.h"
#include "AnimationBuilder.h"
#include "Notification.h"

#include "AnimationClipData.h"

namespace ursine
{
    AnimationState::AnimationState()
        : m_name("")
        , m_timePos(0.0f)
        , m_transPos(0.0f)
        , m_animname("")
        , m_animation(nullptr)
    {}

    const std::string &AnimationState::GetName(void) const
    {
        return m_name;
    }

    void AnimationState::SetName(const std::string& name)
    {
        m_name = name;
    }

    float AnimationState::GetTimePosition(void) const
    {
        return m_timePos;
    }

    void AnimationState::SetTimePosition(const float position)
    {
        m_timePos = position;
    }

    void AnimationState::IncrementTimePosition(const float dt)
    {
        m_timePos += dt;
    }

    // change this to push_back or something that can handle multiple animations insdie
    const Animation* AnimationState::GetAnimation(void) const
    {
        return m_animation;
    }

    void AnimationState::SetAnimation(Animation* animation)
    {
        m_animation = animation;
    }

    const std::string &AnimationState::GetAnimationName(void) const
    {
        return m_animname;
    }

    void AnimationState::SetAnimationName(const std::string& name)
    {
        if ("" == name)
            return;
        m_animname = name;

        Animation* targetAnimation = AnimationBuilder::GetAnimationByName(m_animname);
        if (!targetAnimation)
        {
#if defined(URSINE_WITH_EDITOR)
            NotificationConfig error;

            error.type = NOTIFY_ERROR;
            error.header = "Animation doesn't exist";
            error.message = "To add animation into the state, animation should exist in the Animation List";

            EditorPostNotification(error);
#endif
            return;
        }

        m_animation = targetAnimation;
    }

    // make this can handle multiple names of animation name
    const Animation* AnimationState::GetAnimationByName(void) const
    {
        return m_animation;
    }

    void AnimationState::SetAnimationByName(const std::string& name)
    {
        Animation* targetAnimation = AnimationBuilder::GetAnimationByName(name);
        if (!targetAnimation)
        {
            m_animation = nullptr;
            return;
        }
        m_animation = targetAnimation;
    }

    const resources::ResourceReference &AnimationState::GetClip(void) const
    {
        return m_clipResource;
    }

    void AnimationState::SetClip(const resources::ResourceReference &clip)
    {
        m_clipResource = clip;

        auto data = loadResource<resources::AnimationClipData>(m_clipResource);

        //if (!resourcesAreAvailable())
        //    return;
        //
        //invalidateClip();
        //
        //NOTIFY_COMPONENT_CHANGED("clip", m_clipResource);
    }

    void AnimationState::invalidateClip(void)
    {
        //auto data = loadResource<resources::AnimationClipData>(m_clipResource);
        //
        //std::string str;
        //
        //if (data == nullptr)
        //{
        //    NotificationConfig config;
        //
        //    config.type = NOTIFY_WARNING;
        //    config.header = "Warning";
        //    config.message = "There is no matching animation clip resource.";
        //    config.dismissible = true;
        //    config.duration = TimeSpan::FromSeconds(5.0f);
        //
        //    m_animationName = "";
        //}
        //else
        //{
        //    auto handle = data->GetAnimeHandle();
        //
        //    m_animationName = GetCoreSystem(graphics::GfxAPI)->ResourceMgr.GetAnimInfo(handle)->name;
        //}
    }

    const float &AnimationState::GetTransPosition(void) const
    {
        return m_transPos;
    }

    void AnimationState::SetTransPosition(const float& tPos)
    {
        m_transPos = tPos;
    }
}
