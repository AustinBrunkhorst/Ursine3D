/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleSystemComponent.h
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
#include "Color.h"

namespace ursine
{
    namespace ecs
    {
        class ParticleSystem
            : public Component
            , public RenderableComponentBase
        {
            NATIVE_COMPONENT;

        public:
            //EditorField(
            //    LightType Type,
            //    GetType,
            //    SetType
            //    );


            ParticleSystem(void);
            ~ParticleSystem(void);

            Meta(Disable)
                void OnInitialize(void) override;

            // count
            unsigned GetActiveParticleCount(void) const;
            unsigned GetInactiveParticleCount(void) const;

            // gpu vector
            std::vector<graphics::Particle_GPU> &GetGPUParticleData(void);

            // cpu vector
            std::vector<graphics::Particle_CPU> &GetCPUParticleData(void);

            // generate particle, returns index
            int SpawnParticle(void);

            void DestroyParticle(const int index);

            // command all particle components to operate on the set of particles
            void updateRenderer(void) override;

        private:
            graphics::ParticleSystem *m_particleSystem;

            // color
        } Meta(Enable, DisplayName("ParticleSystem"));
    }
}