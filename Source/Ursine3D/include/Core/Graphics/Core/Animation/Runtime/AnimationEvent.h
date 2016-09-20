/* ---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationEvent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "AnimationState.h"

namespace ursine
{
    class AnimationEvent
    {
    public:
        EditorField(
            std::string stateName,
            GetStateName,
            SetStateName
        );

        EditorMeta(InputRange(0.0f, 1.0f, 0.01f))
        EditorField(
            float ratio,
            GetRatio,
            SetRatio
        );

        EditorField(
            std::string eventMessage,
            GetEventMessage,
            SetEventMessage
        );

        AnimationEvent(void);

        const std::string &GetStateName(void) const;
        void SetStateName(const std::string &state);

        float GetRatio(void) const;
        void SetRatio(float ratio);

        const std::string &GetEventMessage(void) const;
        void SetEventMessage(const std::string &message);

    private:
        friend class ecs::Animator;

        // The state this event is for
        std::string m_stateName;

        // The message this event carries
        std::string m_message;

        // The ratio we're going to send on
        float m_ratio;

        // This is used so we don't send an event twice for a given state
        bool m_sent;

    } Meta(Enable, EnableArrayType, DisplayName("AnimationEvent"));
}
