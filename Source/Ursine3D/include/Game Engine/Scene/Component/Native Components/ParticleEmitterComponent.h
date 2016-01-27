/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleEmitterComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/


#pragma once

#include "Component.h"
#include "Renderable.h"
#include "GfxAPI.h"
#include "RenderableComponentBase.h"
#include "ParticleSystemComponent.h"

#include "SVec3.h"

#include "Randomizer.h"

namespace ursine
{
    namespace ecs
    {
        class ParticleEmitter
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            ParticleEmitter(void);
            ~ParticleEmitter(void);

            Meta(Disable)
                void OnInitialize(void) override;

        private:
            // parent component, this is kinda important
            ParticleSystem *m_particleComponent;

            // all of these need variance /////////////////////////////////////
            // emit speed
            float m_emitSpeed;
            Randomizer m_eminRange;

            // max emit count, 0 is unlimited
             
            // lifetime
            float m_lifetime;
            Randomizer m_lifetimeRange;

            // size
            float m_scale;
            Randomizer m_scaleRange;

            // rotation
            float m_rotation;
            Randomizer m_rotationRange;

            // velocity
            SVec3 m_initialVelocity;
            Randomizer m_xVelRange;
            Randomizer m_yVelRange;
            Randomizer m_zVelRange;

            // emitter size (related to scale of transform?)
            float m_radius;
            Randomizer m_radiusRange;
        } Meta(Enable, DisplayName("ParticleEmitter"));
    }
}