/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace ursine
{
    namespace graphics
    {
        ///////////////////////////////////////////////////////////////////
        // renderable class
        Renderable::Renderable()
        {
            Active_ = false;
        }

        void Renderable::Initialize()
        {
            Overdraw_ = false;
            Debug_ = false;
            m_mask = 0x7FFFFFFFFFFFFFFF;
        }

        void Renderable::SetEntityUniqueID(const ecs::EntityUniqueID id)
        {
            entityID = id;
        }

        ecs::EntityUniqueID Renderable::GetEntityUniqueID() const
        {
            return entityID;
        }

        void Renderable::SetOverdraw(bool draw)
        {
            Overdraw_ = draw;
        }

        bool Renderable::GetOverdraw() const
        {
            return Overdraw_;
        }

        void Renderable::SetDebug(bool debug)
        {
            Debug_ = debug;
        }

        bool Renderable::GetDebug() const
        {
            return Debug_;
        }

        RenderMask Renderable::GetRenderMask(void) const
        {
            return m_mask;
        }

        void Renderable::SetRenderMask(const RenderMask mask)
        {
            m_mask = mask;
        }

        ///////////////////////////////////////////////////////////////////
        //model class
        Model::Model(void)
        {
            Transform_ = SMat4::Identity();
        }

        const SMat4 &Model::GetWorldMatrix(void)
        {
            return Transform_;
        }

        void Model::SetWorldMatrix(const SMat4 &matrix)
        {
            Transform_ = matrix;
        }

        Model3D::Model3D(void)
        {
            m_matrixPalette.resize(MAX_BONE_COUNT);
        }

        ///////////////////////////////////////////////////////////////////
        //model3d
        void Model3D::Initialize(void)
        {
			Renderable::Initialize( );

            ModelName_ = "Cube";
            MaterialName_ = "UV";

            m_emissive = 0.45f;
            m_specPow = 0;
            m_specIntensity = 0;
            SetOverdraw(false);
            SetDebug(false);
            m_color = Color(1, 1, 1, 1);
            m_meshIndex = -1;
        }

        void Model3D::SetMaterialData(float emiss, float pow, float intensity)
        {
            m_emissive = emiss;
            m_specPow = pow;
            m_specIntensity = intensity;
        }

        void Model3D::GetMaterialData(float &emiss, float &pow, float &intensity)
        {
            emiss = m_emissive;
            pow = m_specPow;
            intensity = m_specIntensity;
        }


		void Model3D::SetAnimationTime(const float time)
		{
			m_animationTime = time;
		}

		float & Model3D::GetAnimationTime(void)
		{
			// TODO: insert return statement here
			return m_animationTime;
		}

        void Model3D::SetColor(const Color color)
        {
            m_color = color;
        }

        const Color &Model3D::GetColor() const
        {
            return m_color;
        }

        std::vector<SMat4>& Model3D::GetMatrixPalette()
        {
            return m_matrixPalette;
        }

        int Model3D::GetMeshIndex(void) const
        {
            return m_meshIndex;
        }

        void Model3D::SetMeshIndex(const int index)
        {
            m_meshIndex = index;
        }


        const char *Model3D::GetModelName(void)
        {
            return ModelName_.c_str();
        }

        void Model3D::SetModel(std::string modelName)
        {
            ModelName_ = modelName;
        }

        const char *Model3D::GetMaterialslName(void)
        {
            return MaterialName_.c_str();
        }

        void Model3D::SetMaterial(std::string materialName)
        {
            MaterialName_ = materialName;
        }


        ///////////////////////////////////////////////////////////////////
        //billboard2d
        Billboard2D::Billboard2D(void)
        {
            TextureName_ = "Default";
            m_width = 1;
            m_height = 1;
        }

        const char *Billboard2D::GetTextureName(void)
        {
            return TextureName_.c_str();
        }

        void Billboard2D::SetTexture(std::string texName)
        {
            TextureName_ = texName;
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

        const Color &Billboard2D::GetColor() const
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

            Renderable::Initialize( );
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

        float Light::GetIntensity()
        {
            return m_intensity;
        }

        void Light::SetIntensity(const float intensity)
        {
            m_intensity = intensity;
        }

        const Vec2& Light::GetSpotlightAngles()
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

        const SMat4& Light::GetSpotlightTransform()
        {
            return m_spotlightTransform;
        }

        /////////////////////////////////////////////////////////////
        // PARTICLE SYSTEM //////////////////////////////////////////
        ParticleSystem::ParticleSystem(void)
            : m_backIndex(-1)
        {
        }

        void ParticleSystem::Initialize(void)
        {
            m_backIndex = -1;
            Renderable::Initialize();
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
            return static_cast<unsigned>(m_gpuParticleData.size( ));
        }

        unsigned ParticleSystem::GetActiveParticleCount(void) const
        {
            return m_backIndex > 0 ? static_cast<unsigned>(m_backIndex) : 0;
        }

        unsigned ParticleSystem::GetInactiveParticleCount(void) const
        {
            return static_cast<unsigned>( m_gpuParticleData.size( ) ) - (m_backIndex + 1);
        }

        int ParticleSystem::SpawnParticle(void)
        {
            // no available particles, we need to expand
            // should be amortized
            if ( GetInactiveParticleCount() == 0 )
            {
                // push new particle to the back
                m_gpuParticleData.push_back(Particle_GPU( ));
                m_cpuParticleData.push_back(Particle_CPU());
            }
            
            // bam! allocated
            return m_backIndex++;
        }

        void ParticleSystem::DestroyParticle(const int index)
        {
            // swap with the backmost data
            m_gpuParticleData[ index ] = m_gpuParticleData[ m_backIndex ];
            m_cpuParticleData[ index ] = m_cpuParticleData[ m_backIndex ];

            // decrement
            --m_backIndex;
        }

        const SVec3 & ParticleSystem::GetPosition(void) const
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
    }
}
