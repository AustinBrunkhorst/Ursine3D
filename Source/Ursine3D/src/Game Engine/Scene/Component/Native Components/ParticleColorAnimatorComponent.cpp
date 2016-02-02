/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleColorAnimatorComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "ParticleColorAnimatorComponent.h"
#include "EntityEvent.h"

#define SETCOLOR(floatArray, color) floatArray[0] = color.r; floatArray[1] = color.g; floatArray[2] = color.b; floatArray[3] = color.a; 

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(ParticleColorAnimator);

        ParticleColorAnimator::ParticleColorAnimator(void)
            : BaseComponent()
            , m_particleComponent(nullptr)
            , m_color0(1.0f, 1.0f, 1.0f, 0.5f)
            , m_color1(1.0f, 1.0f, 1.0f, 1.0f)
            , m_color2(1.0f, 1.0f, 1.0f, 1.0f)
            , m_color3(1.0f, 1.0f, 1.0f, 0.75f)
            , m_color4(1.0f, 1.0f, 1.0f, 0.0f)
        {
        }

        ParticleColorAnimator::~ParticleColorAnimator(void)
        {
            GetOwner()->Listener(this)
                .Off(ENTITY_PARTICLE_UPDATE, &ParticleColorAnimator::onParticleUpdate);
        }

        void ParticleColorAnimator::OnInitialize(void)
        {
            Component::OnInitialize();

            m_particleComponent = GetOwner()->GetComponent<ParticleSystem>();

            GetOwner()->Listener(this)
                .On(ENTITY_PARTICLE_UPDATE, &ParticleColorAnimator::onParticleUpdate);
        }

        const Color &ParticleColorAnimator::GetColor0(void) const
        {
            return m_color0;
        }

        void ParticleColorAnimator::SetColor0(const Color &color)
        {
            m_color0 = color;
            NOTIFY_COMPONENT_CHANGED("color0", m_color0);
        }

        const Color &ParticleColorAnimator::GetColor1(void) const
        {
            return m_color1;
        }

        void ParticleColorAnimator::SetColor1(const Color &color)
        {
            m_color1 = color;
            NOTIFY_COMPONENT_CHANGED("color1", m_color1);
        }

        const Color &ParticleColorAnimator::GetColor2(void) const
        {
            return m_color2;
        }

        void ParticleColorAnimator::SetColor2(const Color &color)
        {
            m_color2 = color;
            NOTIFY_COMPONENT_CHANGED("color2", m_color2);
        }

        const Color &ParticleColorAnimator::GetColor3(void) const
        {
            return m_color3;
        }

        void ParticleColorAnimator::SetColor3(const Color &color)
        {
            m_color3 = color;
            NOTIFY_COMPONENT_CHANGED("color3", m_color3);
        }

        const Color &ParticleColorAnimator::GetColor4(void) const
        {
            return m_color4;
        }

        void ParticleColorAnimator::SetColor4(const Color &color)
        {
            m_color4 = color;
            NOTIFY_COMPONENT_CHANGED("color4", m_color4);
        }

        void ParticleColorAnimator::onParticleUpdate(EVENT_HANDLER(Entity))
        {
            // get all the particles
            auto &gpuData = m_particleComponent->GetGPUParticleData();
            auto &cpuData = m_particleComponent->GetCPUParticleData();

            int particleCount = m_particleComponent->GetActiveParticleCount();

            for ( int x = 0; x < particleCount; ++x )
            {
                // calculate the lifetime percentage of this particle
                float percentage = (cpuData[ x ].totalLifetime - cpuData[ x ].lifeTime) / cpuData[ x ].totalLifetime;
                Color finalColor;

                if ( percentage > 0.75f )  // 0.8 - 1
                {
                    float scalar = (percentage - 0.8f) / 0.25f;
                    finalColor = interpolateColor(m_color3, m_color4, scalar);
                }
                else if ( percentage > 0.5f )  // 0.6 - 0.8
                {
                    float scalar = (percentage - 0.6f) / 0.25f;
                    finalColor = interpolateColor(m_color2, m_color3, scalar);
                }
                else if ( percentage > 0.25f )  // 0.4 - 0.6
                {
                    float scalar = (percentage - 0.4f) / 0.25f;
                    finalColor = interpolateColor(m_color1, m_color2, scalar);
                }
                else
                {
                    float scalar = (percentage - 0.2f) / 0.25f;
                    finalColor = interpolateColor(m_color0, m_color1, scalar);
                }

                SETCOLOR(gpuData[ x ].color, finalColor);
            }
        }

        Color ParticleColorAnimator::interpolateColor(const Color & start, const Color & end, const float scalar)
        {
            float startScalar = 1.0f - scalar;

            return Color(
                start.r * startScalar + end.r * scalar, 
                start.g * startScalar + end.g * scalar,
                start.b * startScalar + end.b * scalar,
                start.a * startScalar + end.a * scalar
            );
        }
    }
}