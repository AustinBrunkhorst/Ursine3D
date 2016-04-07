/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShaderBufferManager.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h" 

#include "FragmentationComponent.h"
#include "Model3DComponent.h"

#include "Randomizer.h"

namespace ursine
{
    namespace ecs
    {
        static Randomizer randomizer = Randomizer(-100, 100);

        NATIVE_COMPONENT_DEFINITION(ModelFragmenter)

        ModelFragmenter::ModelFragmenter(void)
            : BaseComponent( )
            , m_isPlaying( false )
        {
        }

        ModelFragmenter::~ModelFragmenter(void)
        {
            m_model->SetDoesFragment( false );
        }

#ifdef URSINE_WITH_EDITOR
        void ModelFragmenter::Play(void)
        {
            m_isPlaying = true;
        }

        void ModelFragmenter::Pause(void)
        {
            m_isPlaying = false;
        }

        void ModelFragmenter::Restart(void)
        {
            m_isPlaying = true;
            ResetFragmentation( );

            NOTIFY_COMPONENT_CHANGED("time", m_model->GetFragmentData( ).time);
            NOTIFY_COMPONENT_CHANGED("timeSlider", m_model->GetFragmentData( ).time / m_model->GetFragmentData( ).maxTime);
        }
#endif

        void ModelFragmenter::OnInitialize(void)
        {
            m_model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            m_graphics = GetCoreSystem( graphics::GfxAPI );

            m_model->SetDoesFragment( true );

            m_model->GetFragmentData( ).seed = randomizer.GetValue( );
        }

        void ModelFragmenter::OnSceneReady(Scene *scene)
        {
            invalidateTexture( false );
        }

        bool ModelFragmenter::GetIsActive(void) const
        {
            return m_model->GetDoesFragment( );
        }

        void ModelFragmenter::SetIsActive(bool isActive)
        {
            m_model->SetDoesFragment( isActive );
        }

        const resources::ResourceReference &ModelFragmenter::GetTexture( ) const
        {
            return m_textureResource;
        }

        void ModelFragmenter::SetTexture(const resources::ResourceReference &texture)
        {
            m_textureResource = texture;

            if (!resourcesAreAvailable( ))
                return;

            invalidateTexture( );

            NOTIFY_COMPONENT_CHANGED("texture", m_textureResource);
        }

        float ModelFragmenter::GetTime(void) const
        {
            return m_model->GetFragmentData( ).time;
        }

        void ModelFragmenter::SetTime(float value)
        {
            m_model->GetFragmentData( ).time = value;

            NOTIFY_COMPONENT_CHANGED("time", m_model->GetFragmentData( ).time);
            NOTIFY_COMPONENT_CHANGED("timeSlider", m_model->GetFragmentData( ).time / m_model->GetFragmentData( ).maxTime);
        }

        float ModelFragmenter::GetMaxTime(void) const
        {
            return m_model->GetFragmentData( ).maxTime;
        }

        void ModelFragmenter::SetMaxTime(float value)
        {
            m_model->GetFragmentData( ).maxTime = value;

            NOTIFY_COMPONENT_CHANGED("timeSlider", m_model->GetFragmentData( ).time / m_model->GetFragmentData( ).maxTime);
        }

        float ModelFragmenter::GetVerticalForce(void) const
        {
            return m_model->GetFragmentData( ).verticalForce;
        }

        void ModelFragmenter::SetVerticalForce(float value)
        {
            m_model->GetFragmentData( ).verticalForce = value;
        }

        float ModelFragmenter::GetHorizontalForce(void) const
        {
            return m_model->GetFragmentData( ).horizontalForce;
        }

        void ModelFragmenter::SetHorizontalForce(float value)
        {
            m_model->GetFragmentData( ).horizontalForce = value;
        }

        float ModelFragmenter::GetOutwardForce(void) const
        {
            return m_model->GetFragmentData( ).outwardForce;
        }

        void ModelFragmenter::SetOutwardForce(float value)
        {
            m_model->GetFragmentData( ).outwardForce = value;
        }

        float ModelFragmenter::GetGravityForce(void) const
        {
            return m_model->GetFragmentData( ).gravityForce;
        }

        void ModelFragmenter::SetGravityForce(float value)
        {
            m_model->GetFragmentData( ).gravityForce = value;
        }

        float ModelFragmenter::GetRandomForce(void) const
        {
            return m_model->GetFragmentData( ).randomForce;
        }

        void ModelFragmenter::SetRandomForce(float value)
        {
            m_model->GetFragmentData( ).randomForce = value;
        }

        float ModelFragmenter::GetPulseSpeed(void) const
        {
            return m_model->GetFragmentData( ).pulseSpeed;
        }

        void ModelFragmenter::SetPulseSpeed(float value)
        {
            m_model->GetFragmentData( ).pulseSpeed = value;
        }

        float ModelFragmenter::GetFadeAmount(void) const
        {
            return m_model->GetFragmentData( ).fadeAmount;
        }

        void ModelFragmenter::SetFadeAmount(float value)
        {
            m_model->GetFragmentData( ).fadeAmount = value;
        }

        float ModelFragmenter::GetTransparencyThreshold(void) const
        {
            return m_model->GetFragmentData( ).transparencyThreshold;
        }

        void ModelFragmenter::SetTransparencyThreshold(float value)
        {
            m_model->GetFragmentData( ).transparencyThreshold = value;
        }

        float ModelFragmenter::GetNormalOffset(void) const
        {
            return m_model->GetFragmentData( ).normalOffset;
        }

        void ModelFragmenter::SetNormalOffset(float value)
        {
            m_model->GetFragmentData( ).normalOffset = value;
        }

        float ModelFragmenter::GetSpinScalar(void) const
        {
            return m_model->GetFragmentData( ).spinScalar;
        }

        void ModelFragmenter::SetSpinScalar(float value)
        {
            m_model->GetFragmentData( ).spinScalar = value;
        }

        Vec2 ModelFragmenter::GetTextureUV(void) const
        {
            return Vec2( m_model->GetFragmentData( ).xUV, m_model->GetFragmentData( ).yUV );
        }

        void ModelFragmenter::SetTextureUV(Vec2 value)
        {
            m_model->GetFragmentData().xUV = value.X( );
            m_model->GetFragmentData().yUV = value.Y( );
        }

        void ModelFragmenter::Update(float dt)
        {
            if(m_isPlaying)
                m_model->GetFragmentData( ).time += dt;
        }

        void ModelFragmenter::ResetFragmentation(void)
        {
            m_model->GetFragmentData( ).seed = randomizer.GetValue( );
            m_model->GetFragmentData( ).time = 0;
        }

        float ModelFragmenter::GetTimeSliderValue() const
        {
            return m_model->GetFragmentData( ).time / m_model->GetFragmentData( ).maxTime;
        }

        void ModelFragmenter::SetTimeSliderValue(float value)
        {
            m_model->GetFragmentData().time = value * m_model->GetFragmentData( ).maxTime;
            NOTIFY_COMPONENT_CHANGED("time", m_model->GetFragmentData().time);
        }

        void ModelFragmenter::invalidateTexture(bool unload)
        {
            auto data = loadResource<resources::TextureData>( m_textureResource );

            if (data == nullptr)
            {
                // default
                m_model->SetFragmentTextureHandle( 1 );
            }
            else
            {
                auto handle = data->GetTextureHandle( );

                if (unload)
                    m_graphics->ResourceMgr.UnloadTexture( m_model->GetFragmentTextureHandle( ) );

                m_graphics->ResourceMgr.LoadTexture( handle );

                m_model->SetFragmentTextureHandle( handle );
            }
        }

        void ModelFragmenter::onTextureReload(void)
        {
            invalidateTexture( false );
        }
    }
}