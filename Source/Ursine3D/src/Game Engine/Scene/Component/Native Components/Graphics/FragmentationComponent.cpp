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
            , m_playReverse( false )
        {
            m_graphics = GetCoreSystem(graphics::GfxAPI);
        }

        ModelFragmenter::~ModelFragmenter(void)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->SetDoesFragment( false );
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

            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            NOTIFY_COMPONENT_CHANGED( "time", model->GetFragmentData( ).time );
            NOTIFY_COMPONENT_CHANGED( "timeSlider", model->GetFragmentData( ).time / model->GetFragmentData( ).maxTime );
        }
#endif

        void ModelFragmenter::OnInitialize(void)
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            model->SetDoesFragment( true );

            model->GetFragmentData( ).seed = randomizer.GetValue( );
        }

        void ModelFragmenter::OnSceneReady(Scene *scene)
        {
            invalidateTexture( false );
        }

        bool ModelFragmenter::GetIsActive(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetDoesFragment( );
        }

        void ModelFragmenter::SetIsActive(bool isActive)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->SetDoesFragment( isActive );
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

            NOTIFY_COMPONENT_CHANGED( "texture", m_textureResource );
        }

        float ModelFragmenter::GetTime(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).time;
        }

        void ModelFragmenter::SetTime(float value)
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;
            model->GetFragmentData( ).time = value;

            NOTIFY_COMPONENT_CHANGED( "time", model->GetFragmentData( ).time );
            NOTIFY_COMPONENT_CHANGED( "timeSlider", model->GetFragmentData( ).time / model->GetFragmentData( ).maxTime );
        }

        float ModelFragmenter::GetMaxTime(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).maxTime;
        }

        void ModelFragmenter::SetMaxTime(float value)
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            model->GetFragmentData( ).maxTime = value;

            NOTIFY_COMPONENT_CHANGED( "timeSlider", model->GetFragmentData( ).time / model->GetFragmentData( ).maxTime );
        }

        float ModelFragmenter::GetVerticalForce(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).verticalForce;
        }

        void ModelFragmenter::SetVerticalForce(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).verticalForce = value;

            NOTIFY_COMPONENT_CHANGED( "verticalForce", value );
        }

        float ModelFragmenter::GetHorizontalForce(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).horizontalForce;
        }

        void ModelFragmenter::SetHorizontalForce(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).horizontalForce = value;
            NOTIFY_COMPONENT_CHANGED( "horizontalForce", value );
        }

        float ModelFragmenter::GetOutwardForce(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).outwardForce;
        }

        void ModelFragmenter::SetOutwardForce(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).outwardForce = value;

            NOTIFY_COMPONENT_CHANGED( "outwardForce", value );
        }

        float ModelFragmenter::GetGravityForce(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).gravityForce;
        }

        void ModelFragmenter::SetGravityForce(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).gravityForce = value;

            NOTIFY_COMPONENT_CHANGED( "gravityForce", value );
        }

        float ModelFragmenter::GetRandomForce(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).randomForce;
        }

        void ModelFragmenter::SetRandomForce(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).randomForce = value;
            NOTIFY_COMPONENT_CHANGED( "randomForce", value );
        }

        float ModelFragmenter::GetPulseSpeed(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).pulseSpeed;
        }

        void ModelFragmenter::SetPulseSpeed(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).pulseSpeed = value;
            
            NOTIFY_COMPONENT_CHANGED( "pulseSpeed", value );
        }

        float ModelFragmenter::GetFadeAmount(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).fadeAmount;
        }

        void ModelFragmenter::SetFadeAmount(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).fadeAmount = value;
            
            NOTIFY_COMPONENT_CHANGED( "fadeAmount", value );
        }

        float ModelFragmenter::GetTransparencyThreshold(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).transparencyThreshold;
        }

        void ModelFragmenter::SetTransparencyThreshold(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).transparencyThreshold = value;
            
            NOTIFY_COMPONENT_CHANGED( "transparencyThreshold", value );
        }

        float ModelFragmenter::GetNormalOffset(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).normalOffset;
        }

        void ModelFragmenter::SetNormalOffset(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).normalOffset = value;
            
            NOTIFY_COMPONENT_CHANGED( "normalOffset", value );
        }

        float ModelFragmenter::GetSpinScalar(void) const
        {
            return GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).spinScalar;
        }

        void ModelFragmenter::SetSpinScalar(float value)
        {
            GetOwner( )->GetComponent<Model3D>( )->m_model->GetFragmentData( ).spinScalar = value;

            NOTIFY_COMPONENT_CHANGED( "spinScalar", value );
        }

        Vec2 ModelFragmenter::GetTextureUV(void) const
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            return Vec2( model->GetFragmentData( ).xUV, model->GetFragmentData( ).yUV );
        }

        void ModelFragmenter::SetTextureUV(Vec2 value)
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            model->GetFragmentData().xUV = value.X( );
            model->GetFragmentData().yUV = value.Y( );

            NOTIFY_COMPONENT_CHANGED( "textureOffset", value );
        }

        void ModelFragmenter::Update(float dt)
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;
            if(m_isPlaying)
            {
                SetTime( model->GetFragmentData( ).time + dt * (m_playReverse ? -1.0f : 1.0f) );

                if(model->GetFragmentData( ).time < 0)
                    SetTime( 0 );

                if(model->GetFragmentData( ).time > model->GetFragmentData( ).maxTime)
                    SetTime( model->GetFragmentData( ).maxTime );
            }

            model->GetFragmentData( ).globalTime += dt;
        }

        void ModelFragmenter::ResetFragmentation(void)
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            model->GetFragmentData( ).seed = randomizer.GetValue( );
            model->GetFragmentData( ).time = m_playReverse ? model->GetFragmentData( ).maxTime : 0.0f;
        }

        float ModelFragmenter::GetTimeSliderValue() const
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            return model->GetFragmentData( ).time / model->GetFragmentData( ).maxTime;
        }

        void ModelFragmenter::SetTimeSliderValue(float value)
        {
            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            model->GetFragmentData().time = value * model->GetFragmentData( ).maxTime;
            NOTIFY_COMPONENT_CHANGED( "time", model->GetFragmentData().time );
        }

        void ModelFragmenter::StartFragmentation(void)
        {
            m_isPlaying = true;
        }

        void ModelFragmenter::PauseFragmentation(void)
        {
            m_isPlaying = false;
        }

        bool ModelFragmenter::GetPlayInReverse() const
        {
            return m_playReverse;
        }

        void ModelFragmenter::SetPlayInReverse(bool playReverse)
        {
            m_playReverse = playReverse;
            NOTIFY_COMPONENT_CHANGED( "playingBackwards", playReverse );
        }

        void ModelFragmenter::invalidateTexture(bool unload)
        {
            auto data = loadResource<resources::TextureData>( m_textureResource );

            auto *model = GetOwner( )->GetComponent<Model3D>( )->m_model;

            if (data == nullptr)
            {
                // default
                model->SetFragmentTextureHandle( 1 );
            }
            else
            {
                auto handle = data->GetTextureHandle( );

                if (unload)
                    m_graphics->ResourceMgr.UnloadTexture( model->GetFragmentTextureHandle( ) );

                m_graphics->ResourceMgr.LoadTexture( handle );

                model->SetFragmentTextureHandle( handle );
            }
        }

        void ModelFragmenter::onTextureReload(void)
        {
            invalidateTexture( false );
        }
    }
}