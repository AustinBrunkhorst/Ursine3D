/*----------------------------------------------------------------------------
* *Team Bear King
* *© 2015 DigiPen Institute of Technology, All Rights Reserved.
**
* *AnimatorComponent.cpp
**
**Author:
**- Jordan Ellis - j.ellis@digipen.edu
*
**Contributors:
**- <list in same format as author if applicable>
**--------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AnimatorComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(Animator);

        Animator::Animator(void)
            : BaseComponent( )
            , m_playing( true )
            , m_time( 0.0f )
            , m_direction( 1.0f )
            , m_playmode( AnimationPlaymode::Loop ) { }

        const resources::ResourceReference& Animator::GetAnimation() const
        {
            return m_animation;
        }

        void Animator::SetAnimation(const resources::ResourceReference& animation)
        {
            m_animation = animation;

            NOTIFY_COMPONENT_CHANGED( "animation", m_animation );
        }

        bool Animator::GetPlaying(void) const
        {
            return m_playing;
        }

        void Animator::SetPlaying(bool playing)
        {
            m_playing = playing;

            NOTIFY_COMPONENT_CHANGED( "playing", m_playing );
        }

        AnimationPlaymode Animator::GetPlaymode(void) const
        {
            return m_playmode;
        }

        void Animator::SetPlaymode(AnimationPlaymode playmode)
        {
            if (m_playmode != playmode)
            {
                m_direction = 1.0f;
                m_time = 0.0f;
            }

            m_playmode = playmode;

            NOTIFY_COMPONENT_CHANGED( "playmode", m_playmode );
        }
    }
}
