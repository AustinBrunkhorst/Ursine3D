/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UnloadResourceComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "UnloadResourceComponent.h"

#include "LevelSegmentManagerComponent.h"

NATIVE_COMPONENT_DEFINITION( UnloadResource )

using namespace ursine;
using namespace ecs;

UnloadResource::UnloadResource(void)
    : BaseComponent( )
{
}

UnloadResource::UnloadResource(LevelSegmentManager *manager, LevelSegments unloadSegment)
    : BaseComponent( )
    , m_unloadSegment( unloadSegment )
{
    if (manager->GetCurrentSegment( ) >= m_unloadSegment)
        GetOwner( )->Delete( );
    else
        manager->Listener( this )
            .On( LevelSegmentManagerEvents::SegmentChanged, &UnloadResource::onSegmentChange );
}

LevelSegments UnloadResource::GetUnloadSegment(void) const
{
    return m_unloadSegment;
}

void UnloadResource::SetUnloadSegment(LevelSegments segment)
{
    m_unloadSegment = segment;
}

void UnloadResource::onSegmentChange(EVENT_HANDLER(LevelSegmentManager))
{
    EVENT_ATTRS(LevelSegmentManager, LevelSegmentChangeArgs);

    // Unload myself if we've changed to my unload segment
    if (m_unloadSegment == args->segment)
        GetOwner( )->Delete( );
}
