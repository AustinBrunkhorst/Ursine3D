/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** VisibilityTriggerComponent.cpp
**
** Author:
** - Jason Burch - j.orion@digipen.edu
**
** Contributors:
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "VisibilityTriggerComponent.h"
#include "LevelSegmentManagerComponent.h"
#include "Model3DComponent.h"
#include "RigidbodyComponent.h"
#include "ParticleEmitterComponent.h"
#include <Core\CoreSystem.h>
#include <Filter.h>
#include <CollisionEventArgs.h>
#include <World.h>

NATIVE_COMPONENT_DEFINITION(VisibilityTrigger);

using namespace ursine;
using namespace ecs;

VisibilityTrigger::VisibilityTrigger(void)
    : BaseComponent()
    , m_segment(LevelSegments::Empty)
    , m_unsubscribed( true )
    , m_type(VisibilityTriggerType::VIS_MODEL)
    , m_delay( 0.0f )
{
}

VisibilityTrigger::~VisibilityTrigger(void)
{
    if (!m_unsubscribed)
    {
        auto world = GetOwner( )->GetWorld( );

        if (!world)
            return;

        auto segmentManagers = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

        if (segmentManagers.size( ) == 0)
            return;

        auto segmentManager = segmentManagers[ 0 ];

        segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
            .Off( LevelSegmentManagerEvents::SegmentChanged, &VisibilityTrigger::onSegmentChange );
    }

}

void VisibilityTrigger::OnInitialize(void)
{
    auto world = GetOwner( )->GetWorld( );

    if (!world)
        return;

    auto segmentManagers = world->GetEntitiesFromFilter( Filter( ).All<LevelSegmentManager>( ) );

    if (segmentManagers.size( ) == 0)
        return;

    auto segmentManager = segmentManagers[ 0 ];

    segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
        .On( LevelSegmentManagerEvents::SegmentChanged, &VisibilityTrigger::onSegmentChange );

    m_unsubscribed = false;
}

LevelSegments VisibilityTrigger::GetLevelSegment(void) const
{
    return m_segment;
}

float VisibilityTrigger::GetDelayInSec() const
{
    return m_delay;
}

VisibilityTriggerType VisibilityTrigger::GetTriggerType() const
{
    return m_type;
}

void VisibilityTrigger::SetTriggerType(VisibilityTriggerType type)
{
    m_type = type;
}

void VisibilityTrigger::SetDelayInSec(float delay)
{
    m_delay = math::Clamp( delay, 0.0f, 1000.0f );
}

void VisibilityTrigger::SetLevelSegment(LevelSegments levelSegment)
{
    m_segment = levelSegment;
}

void VisibilityTrigger::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    if (m_segment == LevelSegments::Empty)
        return;

    if (args->segment == m_segment)
    {
        Timer::Create( TimeSpan::FromSeconds( m_delay ), kTimerGroupGameplay ).Completed( [=]
        {
            if (m_type == VisibilityTriggerType::VIS_MODEL)
            {
                if (GetOwner( )->HasComponent<Model3D>( ))
                {
                    auto model = GetOwner()->GetComponent<Model3D>( );
                    auto flag = model->GetActive( );
                    model->SetActive( !flag );


                    if (GetOwner( )->HasComponent<Rigidbody>( ))
                    {
                        auto rigid = GetOwner( )->GetComponent<Rigidbody>( );
                        auto rflag = rigid->GetDisableContactResponse( );
                        rigid->SetDisableContactResponse( !rflag );
                    }

                    auto world = GetOwner( )->GetWorld( );

                    if (!world)
                        return;

                    auto segmentManagers = world->GetEntitiesFromFilter( Filter( ).
                        All<LevelSegmentManager>( ) );

                    if (segmentManagers.size( ) == 0)
                        return;

                    auto segmentManager = segmentManagers[ 0 ];

                    segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
                        .Off( LevelSegmentManagerEvents::SegmentChanged, 
                            &VisibilityTrigger::onSegmentChange );

                    m_unsubscribed = true;
                }
            }
            else
            {
                if (GetOwner( )->HasComponent<ParticleEmitter>( ))
                {
                    auto emitter = GetOwner( )->GetComponent<ParticleEmitter>( );
                    auto flag = emitter->GetEmitting( );
                    emitter->SetEmitting( !flag );

                    auto world = GetOwner( )->GetWorld( );

                    if (!world)
                        return;

                    auto segmentManagers = world->GetEntitiesFromFilter( Filter( )
                        .All<LevelSegmentManager>( ) );

                    if (segmentManagers.size( ) == 0)
                        return;

                    auto segmentManager = segmentManagers[ 0 ];

                    segmentManager->GetComponent<LevelSegmentManager>( )->Listener( this )
                        .Off( LevelSegmentManagerEvents::SegmentChanged, &VisibilityTrigger::onSegmentChange );

                    m_unsubscribed = true;
                }
            }
            
        } );

    }
}
