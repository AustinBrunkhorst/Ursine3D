﻿/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimatorComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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
            , m_playing( false )
            , m_looping( true )
            , m_debug( false )
            , m_speedScalar( 1.0f )
            , m_currentAnimation( "" )
            , m_currentRig( "" )
			, m_currentState("")
			, m_stateName( "" )
			, m_animationName( "" )
        {
        }

        Animator::~Animator()
        {
			m_states.clear();
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
			auto *currentAnimation = m_states[m_currentState].GetAnimation();
            auto *rig = AnimationBuilder::GetAnimationRigByName( m_currentRig );
			
            if ( currentAnimation == nullptr || rig == nullptr)
                return;
			
            if ( currentAnimation->GetDesiredBoneCount() != rig->GetBoneCount() )
                return;
            
            auto &matrixPalette = GetOwner( )->GetComponent<Model3D>( )->GetMatrixPalette( );
            std::vector<SMat4> tempVec( 100 );
						
            // update time
            if ( m_playing )
            {
				m_states[m_currentState].IncrementTimePosition( dt * m_speedScalar );
			
                unsigned keyframeCount = currentAnimation->GetRigKeyFrameCount( );
                auto &lastFrame = currentAnimation->GetKeyframe( keyframeCount - 1, 0 );
                auto &firstFrame = currentAnimation->GetKeyframe( 0, 0 );
                if (m_states[m_currentState].GetTimePosition( ) > lastFrame.length )
                {
                    
                    // if we need to loop, go back to 0, maybe the first frame time?
                    if ( m_looping )
						m_states[m_currentState].SetTimePosition( firstFrame.length );
                    else
						m_states[m_currentState].SetTimePosition( lastFrame.length );
                }
            }
			
            // generate the matrices
            AnimationBuilder::GenerateAnimationData( 
				m_states[m_currentState],
                rig, 
                matrixPalette, 
                tempVec 
            );
			
            //////////////////////////////////////////////////////////////////
            //// TEMPORARY DEBUG STUFF
            //// render the debug data
            //if ( m_debug )
            //{
            //    auto *graphics = GetCoreSystem( graphics::GfxAPI );
            //    URSINE_TODO( "Remove this when we implement proper animation clips and stuf" )
            //        std::vector<SVec3> bonePoints( 100 );
            //    auto &hierarchy = rig->GetHierarchyTable( );
            //    auto &worldTransform = GetOwner( )->GetTransform( )->GetLocalToWorldMatrix( );
			//
            //    int maxNodeDistance = 0;
			//
            //    size_t boneCount = hierarchy.size( );
			//
            //    // calculate max distance for colors, calculate bone position
            //    for (size_t x = 0; x < boneCount; ++x)
            //    {
            //        // distance
            //        int distance = 0;
            //        int walker = hierarchy[ x ];
			//
            //        while ( walker != -1 )
            //        {
            //            walker = hierarchy[ walker ];
            //            distance++;
            //        }
			//
            //        if ( distance > maxNodeDistance ) maxNodeDistance = distance;
			//
            //        // bone values
            //        bonePoints[ x ] = worldTransform.TransformPoint( tempVec[ x ].TransformPoint( SVec3( 0, 0, 0 ) ) );
            //    }
			//
            //    // render points
            //    graphics->DrawingMgr.SetOverdraw( true );
            //    for ( unsigned x = 0; x < boneCount; ++x )
            //    {
            //        int distance = 0;
            //        int walker = hierarchy[ x ];
			//
            //        while ( walker != -1 )
            //        {
            //            walker = hierarchy[ walker ];
            //            distance++;
            //        }
			//
            //        float interp = (float)distance / (float)maxNodeDistance;
            //        //draw points
            //        
            //        graphics->DrawingMgr.SetColor( Color( 1 * interp, (1.f - interp), 0, 1 ) );
			//
            //        if ( distance == 0 )
            //            graphics->DrawingMgr.SetColor( 0, 0, 1, 1 );
			//
            //        graphics->DrawingMgr.SetSize( 10 );
            //        SVec3 &p = bonePoints[ x ];
            //        graphics->DrawingMgr.DrawPoint( p );
            //    }
			//
            //    // render lines
            //    for ( size_t x = boneCount - 1; x >= 1; --x )
            //    {
            //        SVec3 &p1 = bonePoints[ x ];
            //        SVec3 &p2 = bonePoints[ hierarchy[ x ] ];
			//
            //        graphics->DrawingMgr.DrawLine( p1, p2 );
            //    }
			//
            //    graphics->DrawingMgr.SetOverdraw( false );
            //}            
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
				
		void Animator::AddState(void)
		{
			if ("" == m_stateName)
				return;

			m_states[ m_stateName ].SetName( m_stateName );
			
			auto *gfx = GetCoreSystem(graphics::GfxAPI);			
			auto *world = GetOwner( )->GetWorld( );
			auto *newEntity = world->CreateEntity( m_stateName );			
			auto *newTrans = newEntity->GetTransform( );
			auto *ownerTrans = GetOwner( )->GetTransform( );
			ownerTrans->AddChild( newTrans );

			SetCurrentState( m_stateName );
		}

		void Animator::RemoveState(void)
		{
			if ( "" == m_stateName )
				return;

			auto *gfx = GetCoreSystem( graphics::GfxAPI );
			auto *world = GetOwner( )->GetWorld( );
			Entity* targetEntity = world->GetEntityFromName( m_stateName );
			if ( targetEntity )
			{
				targetEntity->Delete( );
				world->Update( );
			}
			
			for ( auto iter : m_states )
			{
				if ( iter.first == m_stateName )
				{
					m_states.erase( iter.first );
					return;
				}
			}
		}

		const std::string &Animator::GetAnimation(void) const
		{
			return m_animationName;
		}

		void Animator::SetAnimation(const std::string& name)
		{
			m_animationName = name;
		}
		
		void Animator::AddAnimation(void)
		{
			if ( "" == m_animationName )
				return;

			Animation* targetAnimation = AnimationBuilder::GetAnimationByName( m_animationName );
			if ( !targetAnimation )
				return;

			m_states[ m_currentState ].SetAnimation( targetAnimation );

			auto *gfx = GetCoreSystem( graphics::GfxAPI );
			auto *world = GetOwner( )->GetWorld( );
			auto *newEntity = world->CreateEntity( m_animationName );
			auto *newTrans = newEntity->GetTransform( );
			auto *ownerTrans = GetOwner( )->GetTransform( );
			ownerTrans->AddChild( newTrans );
		}

		void Animator::RemoveAnimation(void)
		{
			if ( "" == m_animationName )
				return;

			auto *gfx = GetCoreSystem( graphics::GfxAPI );
			auto *world = GetOwner( )->GetWorld( );
			Entity* targetEntity = world->GetEntityFromName( m_animationName );
			if ( targetEntity )
			{
				targetEntity->Delete( );
				world->Update( );
			}

			m_states[m_currentState].SetAnimation(nullptr);
		}

        const std::string &Animator::GetRig() const
        {
            return m_currentRig;
        }

        void Animator::SetRig(const std::string &rig)
        {
            m_currentRig = rig;
        }

        float Animator::GetAnimationTimePosition( ) const
        {
			for ( auto &x : m_states )
			{
				if ( x.first == m_currentState )
					return x.second.GetTimePosition( );
			}
			return 0.0f;
        }

        void Animator::SetAnimationTimePosition( const float position )
        {
			m_states[ m_currentState ].SetTimePosition( position );
        }
		
		const std::string& Animator::GetCurrentState(void) const
		{
			return m_currentState;
		}

		void Animator::SetCurrentState(const std::string &state)
		{
			m_currentState = state;
		}

		const std::string& Animator::GetStateName(void) const
		{
			return m_stateName;
		}

		void Animator::SetStateName(const std::string &state)
		{
			m_stateName = state;
		}
    }
}