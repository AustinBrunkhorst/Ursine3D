#include "UrsinePrecompiled.h"

#include "AnimatorComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Animator );

        Animator::Animator( )
            : BaseComponent()
            , m_playing( false )
            , m_looping( false )
            , m_animationTime( 0.0f )
            , m_currentAnimation( "" )
        {
        }

        Animator::~Animator()
        {
            
        }

        void Animator::OnInitialize(void)
        {
            
        }

        void Animator::UpdateAnimation(const float dt)
        {
            URSINE_TODO( "Fix animations so they aren't so hard-coded" );
            auto *currentAnimation = AnimationBuilder::GetAnimationByName( m_currentAnimation );
            auto *rig = AnimationBuilder::GetAnimationRigByIndex( 0 );
            
        }

        bool Animator::IsPlaying(void) const
        {
            return m_playing;
        }

        void Animator::SetPlaying(const bool isPlaying)
        {
            m_playing = isPlaying;
        }

        bool Animator::IsLooping( void ) const
        {
            return m_looping;
        }

        void Animator::SetLooping(const bool isLooping)
        {
            m_looping = isLooping;
        }

        const std::string &Animator::GetAnimation( void ) const
        {
            return m_currentAnimation;
        }

        void Animator::SetAnimation(const std::string& name)
        {
            m_currentAnimation = name;
        }
    }
}