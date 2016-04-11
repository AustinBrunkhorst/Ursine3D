/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Renderable.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "Renderable.h"
#include <Core/Graphics/Core/GfxDefines.h>
#include <DirectXMath.h>
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        ///////////////////////////////////////////////////////////////////
        // renderable class
        Renderable::Renderable(void)
        {
            m_active = false;
        }

        void Renderable::Initialize(void)
        {
            m_useOverdraw = false;
            m_useDebugRendering = false;
            m_mask = 0;
        }

        void Renderable::Uninitialize(GfxManager *mgr)
        {
        }

        void Renderable::SetEntityID(ecs::EntityID id)
        {
            m_entityID = id;
        }

        ecs::EntityID Renderable::GetEntityID(void) const
        {
            return m_entityID;
        }

        void Renderable::SetOverdraw(bool draw)
        {
            m_useOverdraw = draw;
        }

        bool Renderable::GetOverdraw(void) const
        {
            return m_useOverdraw;
        }

        void Renderable::SetDebug(bool debug)
        {
            m_useDebugRendering = debug;
        }

        bool Renderable::GetDebug(void) const
        {
            return m_useDebugRendering;
        }

        RenderMask Renderable::GetRenderMask(void) const
        {
            return m_mask;
        }

        void Renderable::SetRenderMask(const RenderMask mask)
        {
            m_mask = mask;
        }

        bool Renderable::GetActive(void) const
        {
            return m_active;
        }

        void Renderable::SetActive(const bool isActive)
        {
            m_active = isActive;
        }

        ///////////////////////////////////////////////////////////////////
        //model class
        Model::Model(void)
        {
            m_transform = SMat4::Identity( );
        }

        const SMat4 &Model::GetWorldMatrix(void)
        {
            return m_transform;
        }

        void Model::SetWorldMatrix(const SMat4 &matrix)
        {
            m_transform = matrix;
        }

        Model3D::Model3D(void)
        {
            m_matrixPalette.resize(MAX_BONE_COUNT);
            m_matrixPaletteIT.resize(MAX_BONE_COUNT);
        }

        ///////////////////////////////////////////////////////////////////
        //model3d
        void Model3D::Initialize(void)
        {
            Renderable::Initialize( );

            m_textureHandle = INTERNAL_BLANK_TEX;
            m_normalHandle = INTERNAL_BLANK_TEX;
            m_modelHandle = 0;

            m_emissive = 0.0f;
            m_specPow = 0;
            m_specIntensity = 0;
            SetOverdraw(false);
            SetDebug(false);
            m_color = Color(1, 1, 1, 1);
            m_meshIndex = -1;
            m_shadowCaster = true;
            m_textureUVOffset = Vec2(0, 0);
            m_textureUVScalar = Vec2(1, 1);

            m_doesFragment = false;

            InitializeFragment( );
        }

        void Model3D::InitializeFragment(void)
        {
            m_fragData.normalOffset = 1.0f;

            m_fragData.verticalForce = 250.0f;
            m_fragData.horizontalForce = 200;
            m_fragData.outwardForce = 100;
            m_fragData.gravityForce = 1.0f;
            m_fragData.randomForce = 150.0f;
            m_fragData.time = 0;

            m_fragData.pulseSpeed = 1.0f;
            m_fragData.fadeAmount = 0.5f;
            m_fragData.maxTime = 5;
            m_fragData.transparencyThreshold = 0.0f;
            m_fragData.spinScalar = 4.0f;
            m_fragData.color = Vec4(1.0f);
        }

        const char *Model3D::GetModelName(void)
        {
            return m_modelResourceName.c_str( );
        }

        void Model3D::SetModelName(std::string modelName)
        {
            m_modelResourceName = modelName;
        }

        GfxHND Model3D::GetModelHandle(void) const
        {
            return m_modelHandle;
        }

        void Model3D::SetModelHandle(GfxHND handle)
        {
            m_modelHandle = handle;
        }

        GfxHND Model3D::GetTextureHandle(void)
        {
            return m_textureHandle;
        }

        void Model3D::SetTextureHandle(GfxHND handle)
        {
            m_textureHandle = handle;
        }

        GfxHND Model3D::GetFragmentTextureHandle(void)
        {
            return m_fragmentTextureHandle;
        }

        void Model3D::SetFragmentTextureHandle(GfxHND handle)
        {
            m_fragmentTextureHandle = handle;
        }

        GfxHND Model3D::GetNormalTextureHandle(void)
        {
            return m_normalHandle;
        }

        void Model3D::SetNormalTextureHandle(GfxHND handle)
        {
            m_normalHandle = handle;
        }

        void Model3D::SetMaterialData(float emiss, float pow, float intensity)
        {
            m_emissive = emiss;
            m_specPow = pow;
            m_specIntensity = intensity;
        }

        void Model3D::GetMaterialData(float &emiss, float &pow, float &intensity) const
        {
            emiss = m_emissive;
            pow = m_specPow;
            intensity = m_specIntensity;
        }

        float Model3D::GetEmissive(void) const
        {
            return m_emissive;
        }

        void Model3D::SetEmissive(float emiss)
        {
            m_emissive = emiss;
        }

        float Model3D::GetSpecularPower(void) const
        {
            return m_specPow;
        }

        void Model3D::SetSpecularPower(float power)
        {
            m_specPow = power;
        }

        float Model3D::GetSpecularIntensity(void) const
        {
            return m_specIntensity;
        }

        void Model3D::SetSpecularIntensity(float intensity)
        {
            m_specIntensity = intensity;
        }

        float & Model3D::GetAnimationTime(void)
        {
            // TODO: insert return statement here
            return m_animationTime;
        }

        void Model3D::SetAnimationTime(const float time)
        {
            m_animationTime = time;
        }

        bool Model3D::GetShadowCaster(void) const
        {
            return m_shadowCaster;
        }

        void Model3D::SetShaderCaster(bool castShadow)
        {
            m_shadowCaster = castShadow;
        }

        const Vec2 &Model3D::GetTextureUVOffset(void) const
        {
            return m_textureUVOffset;
        }

        void Model3D::SetTextureUVOffset(const Vec2 &offset)
        {
            m_textureUVOffset = offset;
        }

        const Vec2 &Model3D::GetTextureUVScalar() const
        {
            return m_textureUVScalar;
        }

        void Model3D::SetTextureUVScalar(const Vec2 &scalar)
        {
            m_textureUVScalar = scalar;
        }

        bool Model3D::GetDoesFragment(void) const
        {
            return m_doesFragment;
        }

        void Model3D::SetDoesFragment(bool doesFragment)
        {
            m_doesFragment = doesFragment;
        }

        float Model3D::GetFragTime(void) const
        {
            return m_fragData.time;
        }

        void Model3D::SetFragTime(float time)
        {
            m_fragData.time = time;
        }

        Model3D::FragmentData &Model3D::GetFragmentData(void)
        {
            return m_fragData;
        }

        const Color &Model3D::GetColor(void) const
        {
            return m_color;
        }

        void Model3D::SetColor(const Color color)
        {
            m_color = color;
        }

        std::vector<SMat4>& Model3D::GetMatrixPalette(void)
        {
            return m_matrixPalette;
        }

        std::vector<SMat4>& Model3D::GetMatrixPaletteIT(void)
        {
            return m_matrixPaletteIT;
        }

        int Model3D::GetMeshIndex(void) const
        {
            return m_meshIndex;
        }

        void Model3D::SetMeshIndex(const int index)
        {
            m_meshIndex = index;
        }

        ///////////////////////////////////////////////////////////////////
        //billboard2d
        Billboard2D::Billboard2D(void)
        {
            m_textureName = "Default";
            m_width = 1;
            m_height = 1;
            m_textureHandle = 0;
        }

        const char *Billboard2D::GetTextureName(void)
        {
            return m_textureName.c_str();
        }

        void Billboard2D::SetTexture(std::string texName)
        {
            m_textureName = texName;
        }

        GfxHND Billboard2D::GetTextureHandle(void) const
        {
            return m_textureHandle;
        }

        void Billboard2D::SetTextureHandle(GfxHND handle)
        {
            m_textureHandle = handle;
        }

        void Billboard2D::SetDimensions(float width, float height)
        {
            m_width = width;
            m_height = height;
        }

        void Billboard2D::GetDimensions(float &width, float &height)
        {
            width = m_width;
            height = m_height;
        }

        void Billboard2D::SetPosition(const ursine::SVec3& position)
        {
            m_position = position;
        }

        const ursine::SVec3& Billboard2D::GetPosition() const
        {
            return m_position;
        }

        void Billboard2D::SetColor(const Color color)
        {
            m_color = color;
        }

        const Color &Billboard2D::GetColor(void) const
        {
            return m_color;
        }

        ///////////////////////////////////////////////////////////////////
        //lights

        ///////////////////////////////////////////////////////////////////
        // universal light
        void Light::Initialize(void)
        {
            m_type = LIGHT_DIRECTIONAL;
            m_position = SVec3(0, 0, 0);
            m_color = Color::White;
            m_radius = 1.0f;
            m_direction = SVec3(0, -1, 0);
            m_intensity = 1.0f;;

            m_spotlightAngles = Vec2(15, 30);

            m_shadowmap = 0;
            m_shadowmapWidth = 1024;
            m_renderShadows = false;

            Renderable::Initialize();
        }

        void Light::Uninitialize(GfxManager *mgr)
        {
            if(m_shadowmap != 0)
                mgr->dxCore->GetDepthMgr( )->DestroyShadowmapDepthTarget( m_shadowmap );
        }

        Light::LightType Light::GetType(void)
        {
            return m_type;
        }

        void Light::SetType(const Light::LightType type)
        {
            m_type = type;
        }

        const SVec3& Light::GetDirection(void)
        {
            return m_direction;
        }

        void Light::SetDirection(const SVec3& dir)
        {
            m_direction = dir;
        }

        void Light::SetDirection(const float x, const float y, const float z)
        {
            m_direction = SVec3(x, y, z);
        }

        const SVec3& Light::GetPosition(void)
        {
            return m_position;
        }

        void Light::SetPosition(const SVec3& position)
        {
            m_position = position;
        }

        void Light::SetPosition(const float x, const float y, const float z)
        {
            m_position = SVec3(x, y, z);
        }

        const Color& Light::GetColor(void)
        {
            return m_color;
        }

        void Light::SetColor(const Color& color)
        {
            m_color = color;
        }

        void Light::SetColor(const float r, const float g, const  float b)
        {
            m_color = Color(r, g, b, 1.0f);
        }

        float Light::GetRadius(void)
        {
            return m_radius;
        }

        void Light::SetRadius(const float radius)
        {
            m_radius = radius;
        }

        float Light::GetIntensity(void)
        {
            return m_intensity;
        }

        void Light::SetIntensity(const float intensity)
        {
            m_intensity = intensity;
        }

        const Vec2& Light::GetSpotlightAngles(void)
        {
            return m_spotlightAngles;
        }

        void Light::SetSpotlightAngles(const Vec2& angles)
        {
            m_spotlightAngles = angles;
        }

        void Light::SetSpotlightAngles(const float inner, const float outer)
        {
            m_spotlightAngles = Vec2(inner, outer);
        }

        void Light::SetSpotlightTransform(const SMat4& transf)
        {
            m_spotlightTransform = transf;
        }

        const SMat4& Light::GetSpotlightTransform() const
        {
            return m_spotlightTransform;
        }

        SMat4 Light::GenerateShadowView(void) const
        {
            auto tempPos = m_position.ToD3D( );
            auto position = DirectX::XMFLOAT4( 
                tempPos.x, 
                tempPos.y, 
                tempPos.z, 
                1.0f 
            );

            auto tempLook = m_direction.ToD3D( );
            auto dir = DirectX::XMFLOAT4( 
                tempLook.x,
                tempLook.y,
                tempLook.z,
                0.0f 
            );

            auto up = DirectX::XMFLOAT4(
                0.0f, 
                1.0f, 
                0.0f, 
                0.0f
            );

            return SMat4(
                DirectX::XMMatrixLookToLH(
                    DirectX::XMLoadFloat4(&position),
                    DirectX::XMLoadFloat4(&dir),
                    DirectX::XMLoadFloat4(&up)
                ) 
            );
        }

        SMat4 Light::GenerateShadowProjection(void) const
        {
            return SMat4( DirectX::XMMatrixPerspectiveFovLH(
                    math::DegreesToRadians( m_spotlightAngles.Y( ) ), 
                    1.0f,
                    m_radius,
                    1.0f
                )
            );
        }

        unsigned Light::GetShadowmapWidth(void) const
        {
            return m_shadowmapWidth;
        }

        void Light::SetShadowmapWidth(unsigned width)
        {
            m_shadowmapWidth = width;
        }

        GfxHND Light::GetShadowmapHandle(void) const
        {
            return m_shadowmap;
        }

        void Light::SetShadowmapHandle(GfxHND handle)
        {
            m_shadowmap = handle;
        }

        bool Light::GetRenderShadows(void) const
        {
            return m_renderShadows;
        }

        void Light::SetRenderShadows(bool renderShadows)
        {
            m_renderShadows = renderShadows;
        }

        /////////////////////////////////////////////////////////////
        // PARTICLE SYSTEM //////////////////////////////////////////
        ParticleSystem::ParticleSystem(void)
            : m_backIndex(-1)
        {
        }

        void ParticleSystem::Initialize(void)
        {
            m_backIndex = 0;
            Renderable::Initialize();
            m_textureHandle = 0;
            m_useAdditive = true;
            m_worldSpace = true;
            m_velocityOrient = false;

            m_fadeScalar = 0.5f;
        }

        std::vector<Particle_GPU>& ParticleSystem::GetGPUParticleData(void)
        {
            return m_gpuParticleData;
        }

        std::vector<Particle_CPU>& ParticleSystem::GetCPUParticleData(void)
        {
            return m_cpuParticleData;
        }

        Particle_GPU &ParticleSystem::GetGPUParticle(const int index)
        {
            return m_gpuParticleData[ index ];
        }

        Particle_CPU &ParticleSystem::GetCPUParticle(const int index)
        {
            return m_cpuParticleData[ index ];
        }

        unsigned ParticleSystem::GetParticleVectorSize(void) const
        {
            return static_cast<unsigned>( m_gpuParticleData.size( ) );
        }

        unsigned ParticleSystem::GetActiveParticleCount(void) const
        {
            return m_backIndex;
        }

        unsigned ParticleSystem::GetInactiveParticleCount(void) const
        {
            return static_cast<unsigned>( m_gpuParticleData.size( ) ) - m_backIndex;
        }

        int ParticleSystem::SpawnParticle(void)
        {
            // no available particles, we need to expand
            // should be amortized
            if (GetInactiveParticleCount( ) == 0)
            {
                // push new particle to the back
                m_gpuParticleData.push_back(Particle_GPU());
                m_cpuParticleData.push_back(Particle_CPU());
            }

            // bam! allocated
            return m_backIndex++;
        }

        void ParticleSystem::DestroyParticle(const int index)
        {
            // decrement
            --m_backIndex;

            // swap with the backmost data
            m_gpuParticleData[ index ] = m_gpuParticleData[ m_backIndex ];
            m_cpuParticleData[ index ] = m_cpuParticleData[ m_backIndex ];

            m_cpuParticleData[ m_backIndex ].lifeTime = -1.0f;
        }

        void ParticleSystem::DestroyAllParticles(void)
        {
            m_backIndex = 0;
        }

        const SVec3 &ParticleSystem::GetPosition(void) const
        {
            return m_position;
        }

        void ParticleSystem::SetPosition(const SVec3 & position)
        {
            m_position = position;
        }

        const Color &ParticleSystem::GetColor(void) const
        {
            return m_particleColor;
        }

        void ParticleSystem::SetColor(const Color &color)
        {
            m_particleColor = color;
        }

        GfxHND ParticleSystem::GetTextureHandle(void) const
        {
            return m_textureHandle;
        }

        void ParticleSystem::SetTextureHandle(GfxHND handle)
        {
            m_textureHandle = handle;
        }

        bool ParticleSystem::GetAdditive(void) const
        {
            return m_useAdditive;
        }

        void ParticleSystem::SetAdditive(const bool useAdditive)
        {
            m_useAdditive = useAdditive;
        }

        bool ParticleSystem::GetSystemSpace(void) const
        {
            return m_worldSpace;
        }

        void ParticleSystem::SetSystemSpace(const bool useWorldCoordinates)
        {
            m_worldSpace = useWorldCoordinates;
        }

        const SMat4& ParticleSystem::GetTransform(void) const
        {
            return m_transform;
        }

        void ParticleSystem::SetTransform(const SMat4& transform)
        {
            m_transform = transform;
        }

        bool ParticleSystem::GetVelocityOrient(void) const
        {
            return m_velocityOrient;
        }

        void ParticleSystem::SetVelocityOrient(bool velocityOrient)
        {
            m_velocityOrient = velocityOrient;
        }

        float ParticleSystem::GetFadeScalar(void) const
        {
            return m_fadeScalar;
        }
        void ParticleSystem::SetFadeScalar(float scalar)
        {
            m_fadeScalar = scalar;
        }

        /////////////////////////////////////////////////////////////
        // SPRITE TEXT //////////////////////////////////////////////
        SpriteText::SpriteText(void)
        {
            Renderable::Initialize();
            Initialize();
        }

        void SpriteText::Initialize(void)
        {
            m_size = 30.0f;
            m_widthScalar = m_heightScalar = 1.0f;
            m_position = SVec3( );
            m_text = "Sprite Text";
            m_ppu = 64.f;
            m_alignment = ALIGN_LEFT;
            m_filter = true;
            m_color = Color::White;
            m_fontHandle = 0;
        }

        float SpriteText::GetSize(void) const
        {
            return m_size;
        }

        void SpriteText::SetSize(float size)
        {
            m_size = size;
        }

        float SpriteText::GetWidth(void) const
        {
            return m_widthScalar;
        }

        void SpriteText::SetWidth(float size)
        {
            m_widthScalar = size;
        }

        float SpriteText::GetHeight(void) const
        {
            return m_heightScalar;
        }

        void SpriteText::SetHeight(float size)
        {
            m_heightScalar = size;
        }

        const SVec3& SpriteText::GetPosition(void) const
        {
            return m_position;
        }

        void SpriteText::SetPosition(const SVec3& position)
        {
            m_position = position;
        }

        const std::string& SpriteText::GetText(void) const
        {
            return m_text;
        }

        void SpriteText::SetText(const std::string& text)
        {
            m_text = text;
        }

        float SpriteText::GetPPU(void) const
        {
            return m_ppu;
        }

        void SpriteText::SetPPU(float ppu)
        {
            m_ppu = ppu;
        }

        SpriteText::Alignment SpriteText::GetAlignment(void) const
        {
            return m_alignment;
        }

        void SpriteText::SetAlignment(Alignment alignment)
        {
            m_alignment = alignment;
        }

        bool SpriteText::GetFilter(void) const
        {
            return m_filter;
        }
        void SpriteText::SetFilter(bool useFilter)
        {
            m_filter = useFilter;
        }

        const Color &SpriteText::GetColor(void) const
        {
            return m_color;
        }
        void SpriteText::SetColor(const Color &color)
        {
            m_color = color;
        }

        GfxHND SpriteText::GetFontHandle(void) const
        {
            return m_fontHandle;
        }
        void SpriteText::SetFontHandle(GfxHND handle)
        {
            m_fontHandle = handle;
        }
    }
}