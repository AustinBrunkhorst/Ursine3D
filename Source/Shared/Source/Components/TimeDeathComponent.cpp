/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TimeDeathComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "TimeDeathComponent.h"

NATIVE_COMPONENT_DEFINITION( TimeDeath );

TimeDeath::TimeDeath( void )
    : BaseComponent( )
    , m_timeBeforeDeath( 5.0f )
{
}

float TimeDeath::GetTimeBeforeDeath( void )
{
    return m_timeBeforeDeath;
}

void TimeDeath::SetTimeBeforeDeath( const float time )
{
    m_timeBeforeDeath = time;
}

void TimeDeath::DecrementTimer( const float dt )
{
    m_timeBeforeDeath -= dt;

    // Delete owner from the world
    if ( m_timeBeforeDeath < 0.0f )
        GetOwner( )->Delete( );
}

