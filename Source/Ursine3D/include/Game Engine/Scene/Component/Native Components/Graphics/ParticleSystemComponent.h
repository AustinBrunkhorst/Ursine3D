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
#include "TextureData.h"

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
            EditorField(
                bool playing,
                GetPlaying,
                SetPlaying
            );

            EditorField(
                Color color,
                GetColor,
                SetColor
            );

            EditorResourceField(
                ursine::resources::TextureData,
                texture,
                GetTexture,
                SetTexture
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

            EditorMeta(BitMaskEditor)
            EditorField(
                ursine::ecs::RenderMask renderMask,
                GetRenderMask,
                SetRenderMask
            );

            Meta(Enable)
            ParticleSystem(void);
            ~ParticleSystem(void);

            void OnInitialize(void) override;
            void OnSceneReady(Scene *scene) override;

            // count
            unsigned GetActiveParticleCount(void) const;
            unsigned GetInactiveParticleCount(void) const;

            // gpu vector
            std::vector<graphics::Particle_GPU> &GetGPUParticleData(void);
            graphics::Particle_GPU &GetGPUParticle(const int index);

            // cpu vector
            std::vector<graphics::Particle_CPU> &GetCPUParticleData(void);
            graphics::Particle_CPU &GetCPUParticle(const int index);

            // generate particle, returns index
            int SpawnParticle(void);

            // destroys a particle, given an index
            void DestroyParticle(const int index);

            bool GetPlaying(void) const;
            void SetPlaying(bool playing);

            const Color &GetColor(void) const;
            void SetColor(const Color &color);

            const resources::ResourceReference &GetTexture(void) const;
            void SetTexture(const resources::ResourceReference &texture);

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

            resources::ResourceReference m_texture;

            // data for this component
            bool m_playing;

            Color m_particleColor;

            SystemSpace m_systemSpace;

            RenderMode m_renderMode;

            void invalidateTexture(bool unload = true);

            // color
        } Meta(Enable, WhiteListMethods, DisplayName( "ParticleSystem" ));
    }
}