/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Renderable.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "EntityConfig.h"
#include "GFXAPIDefines.h"
#include <string>
#include <stack>
#include "SMat4.h"
#include "SVec3.h"
#include "Vec2.h"
#include "Color.h"
#include "Particle.h"

namespace ursine
{
    namespace graphics
    {
        /////////////////////////////////////////////////////////////////
        //main renderable class
        class Renderable
        {
            friend class GfxManager;
            friend class RenderableManager;
        public:
            Renderable(void);

            void Initialize(void);

            void SetEntityID(ecs::EntityID id);
            ecs::EntityID GetEntityID(void) const;

            void SetOverdraw(bool draw);
            bool GetOverdraw() const;

            void SetDebug(bool debug);
            bool GetDebug() const;

            RenderMask GetRenderMask(void) const;
            void SetRenderMask(const RenderMask mask);

            bool GetActive(void) const;
            void SetActive(const bool isActive);

        private:
            virtual void Uninitialize(GfxManager *mgr);

            ecs::EntityID m_entityID;
            bool m_active;
            bool m_useOverdraw;
            bool m_useDebugRendering;
            RenderMask m_mask;
        };

        /////////////////////////////////////////////////////////////////
        //main model class
        class Model : public Renderable
        {
            friend class RenderableManager;
        public:
            const SMat4 &GetWorldMatrix(void);
            void SetWorldMatrix(const SMat4 &matrix);

            Model(void);

        private:
            SMat4 m_transform;
        };

        /////////////////////////////////////////////////////////////////
        //3d model class
        class Model3D : public Model
        {
            friend class RenderableManager;
        public:
            Model3D(void);
            void Initialize(void);

            const char *GetModelName(void);
            void SetModelName(std::string modelName);

            GfxHND GetModelHandle(void) const;
            void SetModelHandle(GfxHND handle);

            GfxHND GetTextureHandle(void);
            void SetTextureHandle(GfxHND handle);

            void GetMaterialData(float &emiss, float &pow, float &intensity) const;
            void SetMaterialData(float emiss, float pow, float intensity);

            float GetEmissive(void) const;
            void SetEmissive(float emiss);

            float GetSpecularPower(void) const;
            void SetSpecularPower(float power);

            float GetSpecularIntensity(void) const;
            void SetSpecularIntensity(float intensity);

            float &GetAnimationTime(void);
            void SetAnimationTime(const float time);

            const Color &GetColor() const;
            void SetColor(const Color color);

            std::vector<SMat4> &GetMatrixPalette(void);

            int GetMeshIndex(void) const;
            void SetMeshIndex(const int index);

            bool GetShadowCaster(void) const;
            void SetShaderCaster(bool castShadow);

            const Vec2 &GetTextureUVOffset(void) const;
            void SetTextureUVOffset(const Vec2 &offset);

        private:
            float m_emissive;
            float m_specPow;
            float m_specIntensity;
            Color m_color;
            std::string m_modelResourceName;

            GfxHND m_modelHandle;
            GfxHND m_textureHandle;

            float m_animationTime;
            std::vector<SMat4> m_matrixPalette;

            Vec2 m_textureUVOffset;

            // for multimaps
            int m_meshIndex;

            bool m_shadowCaster;
        };

        /////////////////////////////////////////////////////////////////
        //2d billboard class
        class Billboard2D : public Renderable
        {
            friend class RenderableManager;
        public:
            Billboard2D(void);

            const char *GetTextureName(void);
            void SetTexture(std::string texName);

            GfxHND GetTextureHandle(void) const;
            void SetTextureHandle(GfxHND handle);

            void SetDimensions(float width, float height);
            void GetDimensions(float &width, float &height);

            void SetPosition(const ursine::SVec3 &position);
            const ursine::SVec3 &GetPosition(void) const;
            
            void SetColor(const Color color);
            const Color &GetColor() const;

        private:
            float m_width;
            float m_height;
            ursine::Vec2 m_scale;
            ursine::SVec3 m_position;
            Color m_color;
            std::string m_textureName;
            GfxHND m_textureHandle;
        };

        /////////////////////////////////////////////////////////////
        // LIGHT ////////////////////////////////////////////////////
        class Light : public Renderable
        {
            friend class RenderableManager;

        public:
            //enum for the different types of lights
            enum LightType
            {
                LIGHT_DIRECTIONAL = 0,
                LIGHT_POINT,
                LIGHT_SPOTLIGHT,

                LIGHT_COUNT
            };

        public:
            void Initialize(void);

            LightType GetType(void);
            void SetType(const LightType type);

            const SVec3 &GetDirection(void);
            void SetDirection(const SVec3 &dir);
            void SetDirection(const float x, const float y, const float z);

            const SVec3 &GetPosition(void);
            void SetPosition(const SVec3 &position);
            void SetPosition(const float x, const  float y, const float z);

            const Color &GetColor(void);
            void SetColor(const Color &color);
            void SetColor(const float r, const float g, const float b);

            float GetRadius(void);
            void SetRadius(const float radius);

            float GetIntensity(void);
            void SetIntensity(const float intensity);

            const Vec2 &GetSpotlightAngles(void);
            void SetSpotlightAngles(const Vec2 &angles);
            void SetSpotlightAngles(const float inner, const float outer);

            void SetSpotlightTransform(const SMat4 &transf);
            const SMat4 &GetSpotlightTransform(void) const;

            SMat4 GenerateViewSpaceShadowTransform(void) const;
            SMat4 GenerateViewSpaceShadowProjection(void) const;

            SMat4 GenerateShadowView(void) const;
            SMat4 GenerateShadowProjection(void) const;

            unsigned GetShadowmapWidth(void) const;
            void SetShadowmapWidth(unsigned width);

            GfxHND GetShadowmapHandle(void) const;
            void SetShadowmapHandle(GfxHND handle);

        private:
            void Uninitialize(GfxManager *mgr) override;

            LightType m_type;
            SVec3 m_position;
            Color m_color;
            float m_radius;
            SVec3 m_direction;
            float m_intensity;

            Vec2 m_spotlightAngles;
            SMat4 m_spotlightTransform;

            unsigned m_shadowmapWidth;
            GfxHND m_shadowmap;
        };

        /////////////////////////////////////////////////////////////
        // PARTICLE SYSTEM //////////////////////////////////////////
        class ParticleSystem : public Renderable
        {
        public:
            ParticleSystem(void);

            void Initialize(void);

            // get the particles
            std::vector<Particle_GPU> &GetGPUParticleData(void);
            std::vector<Particle_CPU> &GetCPUParticleData(void);

            Particle_GPU &GetGPUParticle(const int index);
            Particle_CPU &GetCPUParticle(const int index);

            // get total number of particles
            unsigned GetParticleVectorSize(void) const;

            // get number of active particles
            unsigned GetActiveParticleCount(void) const;

            // get number of inactive particles
            unsigned GetInactiveParticleCount(void) const;

            // attempt to spawn a particle, ret index of new particle
            // -1 if no free particles left
            int SpawnParticle(void);

            void DestroyParticle(const int index);
            void DestroyAllParticles(void);

            const SVec3 &GetPosition(void) const;
            void SetPosition(const SVec3 &position);

            const Color &GetColor(void) const;
            void SetColor(const Color &color);

            GfxHND GetTextureHandle(void) const;
            void SetTextureHandle(GfxHND handle);

            bool GetAdditive(void) const;
            void SetAdditive(const bool useAdditive);

            bool GetSystemSpace(void) const;
            void SetSystemSpace(const bool useWorldCoordinates);

            const SMat4 &GetTransform(void) const;
            void SetTransform(const SMat4 &transform);
        private:
            // members
            unsigned m_backIndex;
            std::vector<Particle_GPU> m_gpuParticleData;
            std::vector<Particle_CPU> m_cpuParticleData;
            SVec3 m_position;

            GfxHND m_textureHandle;

            Color m_particleColor;

            bool m_useAdditive;
            bool m_worldSpace;

            SMat4 m_transform;
        };

        /////////////////////////////////////////////////////////////
        // SPRITE TEXT //////////////////////////////////////////////
        class SpriteText : public Renderable
        {
        public:
            enum Alignment
            {
                ALIGN_LEFT,
                ALIGN_CENTER,
                ALIGN_RIGHT,

                ALIGN_COUNT
            };

        public:
            SpriteText(void);
            void Initialize(void);

            float GetSize(void) const;
            void SetSize(float size);

            float GetWidth(void) const;
            void SetWidth(float size);

            float GetHeight(void) const;
            void SetHeight(float size);

            const SVec3 &GetPosition(void) const;
            void SetPosition(const SVec3 &position);

            const std::string &GetText(void) const;
            void SetText(const std::string &text);

            float GetPPU(void) const;
            void SetPPU(float ppu);

            Alignment GetAlignment(void) const;
            void SetAlignment(Alignment alignment);

            bool GetFilter(void) const;
            void SetFilter(bool useFilter);

            const Color &GetColor(void) const;
            void SetColor(const Color &color);

            GfxHND GetFontHandle(void) const;
            void SetFontHandle(GfxHND handle);

        private:
            float m_size;

            float m_widthScalar;
            float m_heightScalar;

            bool m_filter;

            Color m_color;

            SVec3 m_position;

            std::string m_text;

            float m_ppu;

            Alignment m_alignment;

            GfxHND m_fontHandle;
        };
    }
}
