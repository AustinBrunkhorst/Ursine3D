/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "RenderMask.h"
#include "Color.h"

namespace ursine
{
    namespace ecs
    {
        enum class SystemSpace
        {
            WorldSpace,
            LocalSpace
        } Meta(Enable);

        enum class RenderMode
        {
            Additive,
            Blended
        } Meta(Enable);

        class ParticleSystem 
            : public Component
        {
            NATIVE_COMPONENT;

            friend class RenderSystem;

        public:
            //EditorField(
            //    LightType Type,
            //    GetType,
            //    SetType
            //    );

            EditorField(
                Color color,
                GetColor,
                SetColor
            );

            EditorField(
                std::string texture,
                GetParticleTextureName,
                SetParticleTextureName
            );

            EditorField(
                SystemSpace systemSpace,
                GetSystemSpace,
                SetSystemSpace
            );

            EditorField(
                RenderMode renderMode,
                GetRenderMode,
                SetRenderMode
            );

            Meta(BitMaskEditor)
            EditorField(
                ursine::ecs::RenderMask renderMask,
                GetRenderMask,
                SetRenderMask
            );

            ParticleSystem(void);
            ~ParticleSystem(void);

            Meta(Disable)
            void OnInitialize(void) override;

            // count
            unsigned GetActiveParticleCount(void) const;
            unsigned GetInactiveParticleCount(void) const;

            // gpu vector
            Meta(Disable)
            std::vector<graphics::Particle_GPU> &GetGPUParticleData(void);

            Meta(Disable)
            graphics::Particle_GPU &GetGPUParticle(const int index);

            // cpu vector
            Meta(Disable)
            std::vector<graphics::Particle_CPU> &GetCPUParticleData(void);

            Meta(Disable)
            graphics::Particle_CPU &GetCPUParticle(const int index);

            // generate particle, returns index
            int SpawnParticle(void);

            // destroys a particle, given an index
            void DestroyParticle(const int index);

            const Color &GetColor(void) const;
            void SetColor(const Color &color);

            const std::string &GetParticleTextureName(void) const;
            void SetParticleTextureName(const std::string &texture);

            SystemSpace GetSystemSpace(void) const;
            void SetSystemSpace(const SystemSpace &space);

            RenderMode GetRenderMode(void) const;
            void SetRenderMode(const RenderMode &renderMode);

            ursine::ecs::RenderMask GetRenderMask(void) const;
            void SetRenderMask(ursine::ecs::RenderMask mask);

        private:
            // command all particle components to operate on the set of particles
            void updateRenderer(void);

            graphics::ParticleSystem *m_particleSystem;

            // The graphics core API
            graphics::GfxAPI *m_graphics;

            RenderableComponentBase *m_base;

            // data for this component
            Color m_particleColor;

            SystemSpace m_systemSpace;

            RenderMode m_renderMode;

            // color
        } Meta(Enable, DisplayName("ParticleSystem"));
    }
}