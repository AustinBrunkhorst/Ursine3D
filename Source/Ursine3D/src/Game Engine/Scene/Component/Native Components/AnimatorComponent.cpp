#include "UrsinePrecompiled.h"

#include "AnimatorComponent.h"
#include <Game Engine/Scene/Component/Native Components/Model3DComponent.h>

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Animator );

        Animator::Animator( )
            : BaseComponent( )
            , m_state( )
            , m_playing( false )
            , m_looping( true )
            , m_debug( false )
            , m_speedScalar( 1.0f )
            , m_currentAnimation( "Take 001" )
        {
        }

        Animator::~Animator()
        {
            
        }

        void Animator::OnInitialize(void)
        {
            // clean up our model's matrices
            auto &matrixPalette = GetOwner( )->GetComponent<Model3D>( )->GetMatrixPalette( );
            for ( auto &x : matrixPalette )
            {
                x = SMat4::Identity( );
            }
        }

        void Animator::UpdateAnimation(const float dt)
        {
            URSINE_TODO( "Fix animations so they aren't so hard-coded" );

            // grab what we need
            auto *currentAnimation = AnimationBuilder::GetAnimationByName( m_currentAnimation );
            auto *rig = AnimationBuilder::GetAnimationRigByIndex( 0 );

            if ( currentAnimation == nullptr || rig == nullptr )
                return;
            
            auto &matrixPalette = GetOwner( )->GetComponent<Model3D>( )->GetMatrixPalette( );
            std::vector<SMat4> tempVeec( 100 );

            // update time
            if ( m_playing )
            {
                m_state.IncrementTimePosition( dt );

                unsigned keyframeCount = currentAnimation->GetRigKeyFrameCount( );
                auto &lastFrame = currentAnimation->GetKeyframe( keyframeCount - 1, 0 );
                auto &firstFrame = currentAnimation->GetKeyframe( 0, 0 );
                if ( m_state.GetTimePosition( ) > lastFrame.length )
                {
                    
                    // if we need to loop, go back to 0, maybe the first frame time?
                    if ( m_looping )
                        m_state.SetTimePosition( firstFrame.length );
                    else
                        m_state.SetTimePosition( lastFrame.length );
                }
            }

            // generate the matrices
            AnimationBuilder::GenerateAnimationData( 
                m_state, 
                rig, 
                matrixPalette, 
                tempVeec 
            );

            ////////////////////////////////////////////////////////////////
            // TEMPORARY DEBUG STUFF
            // render the debug data
            if ( m_debug )
            {
                auto *graphics = GetCoreSystem( graphics::GfxAPI );
                URSINE_TODO( "Remove this when we implement proper animation clips and stuf" )
                    std::vector<SVec3> bonePoints( 100 );
                auto &hierarchy = rig->GetHierarchyTable( );
                auto &worldTransform = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );

                int maxNodeDistance = 0;
                unsigned boneCount = hierarchy.size( );

                // calculate max distance for colors, calculate bone position
                for ( unsigned x = 0; x < boneCount; ++x )
                {
                    // distance
                    int distance = 0;
                    int walker = hierarchy[ x ];

                    while ( walker != -1 )
                    {
                        walker = hierarchy[ walker ];
                        distance++;
                    }

                    if ( distance > maxNodeDistance ) maxNodeDistance = distance;

                    // bone values
                    bonePoints[ x ] = worldTransform.TransformPoint( tempVeec[ x ].TransformPoint( SVec3( 0, 0, 0 ) ) );
                }

                // render points
                graphics->DrawingMgr.SetOverdraw( true );
                for ( unsigned x = 0; x < boneCount; ++x )
                {
                    int distance = 0;
                    int walker = hierarchy[ x ];

                    while ( walker != -1 )
                    {
                        walker = hierarchy[ walker ];
                        distance++;
                    }

                    float interp = (float)distance / (float)maxNodeDistance;
                    //draw points
                    
                    graphics->DrawingMgr.SetColor( Color( 1 * interp, (1.f - interp), 0, 1 ) );

                    if ( distance == 0 )
                        graphics->DrawingMgr.SetColor( 0, 0, 1, 1 );

                    graphics->DrawingMgr.SetSize( 10 );
                    SVec3 &p = bonePoints[ x ];
                    graphics->DrawingMgr.DrawPoint( p );
                }

                // render lines
                for ( unsigned x = boneCount - 1; x >= 1; --x )
                {
                    SVec3 &p1 = bonePoints[ x ];
                    SVec3 &p2 = bonePoints[ hierarchy[ x ] ];

                    graphics->DrawingMgr.DrawLine( p1, p2 );
                }

                graphics->DrawingMgr.SetOverdraw( false );
            }
            
        }

        bool Animator::IsPlaying(void) const
        {
            return m_playing;
        }

        void Animator::SetPlaying(const bool isPlaying)
        {
            m_playing = isPlaying;
        }

        bool Animator::IsLooping( void ) const
        {
            return m_looping;
        }

        void Animator::SetLooping(const bool isLooping)
        {
            m_looping = isLooping;
        }

        bool Animator::IsDebug() const
        {
            return m_debug;
        }

        void Animator::SetDebug(const bool useDebug)
        {
            m_debug = useDebug;
        }

        float Animator::GetTimeScalar() const
        {
            return m_speedScalar;
        }

        void Animator::SetTimeScalar(const float scalar)
        {
            m_speedScalar = scalar;
        }

        const std::string &Animator::GetAnimation( void ) const
        {
            return m_currentAnimation;
        }

        void Animator::SetAnimation(const std::string& name)
        {
            m_currentAnimation = name;
            m_state.SetAnimation( AnimationBuilder::GetAnimationByName( name ) );
        }

        float Animator::GetAnimationTimePosition( ) const
        {
            return m_state.GetTimePosition( );
        }

        void Animator::SetAnimationTimePosition( const float position )
        {
            m_state.SetTimePosition( position );
        }
    }
}