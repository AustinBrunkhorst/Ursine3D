/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BuffComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "BuffComponent.h"
#include "ComponentIncludes.h"

NATIVE_COMPONENT_DEFINITION( BuffComponent ) ;

using namespace ursine;

const float BuffComponent::BUFF_INFINITE = -100.0f;

BuffComponent::BuffComponent(void) :
    BaseComponent( ),
    m_buffStatus{ false },
    m_buffModifiers{ 1.0f },
    m_buffLifeTimes{ 0.0f }
{
}

BuffComponent::~BuffComponent(void)
{
    //GetOwner( )->Listener( this )
    //     .Off( game::FIRE_END, &BaseWeapon::TriggerReleased ); 
}

void BuffComponent::UpdateBuffTimes(const float dt)
{
    for (int i = 0; i < BUFF_COUNT; ++i)
    {
        if ( m_buffLifeTimes[ i ] > 0.0f )
        {
            m_buffLifeTimes[ i ] -= dt;

            if ( m_buffLifeTimes[ i ] <= 0.0f )
                remove( i );
        }
    }
}

void BuffComponent::AddBuff(BuffType type, const float modifier, const float lifeTime)
{
    m_buffStatus[ type ] = true;
    m_buffModifiers[ type ] *= modifier;
    m_buffLifeTimes[ type ] = lifeTime;
}


///////////////////////////////
////     Buff Removals     ////
///////////////////////////////

void BuffComponent::remove(int i)
{
    m_buffStatus[ i ] = false;
    m_buffModifiers[ i ] = 1.0f;
    m_buffLifeTimes[ i ] = 0.0f;
}

void BuffComponent::RemoveBuff(BuffType type)
{
    m_buffStatus[ type ] = false;
}

void BuffComponent::RemoveAllBuffs(void)
{
    for (int i = 0; i < BUFF_COUNT; ++i)
    {
        remove( i );
    }
}

void BuffComponent::RemoveAllInfiniteBuffs(void)
{
    for (int i = 0; i < BUFF_COUNT; ++i)
    {
        if ( m_buffLifeTimes[ i ] == BUFF_INFINITE )
            remove( i );
    }
}

void BuffComponent::RemoveAllTimedBuffs(void)
{
    for (int i = 0; i < BUFF_COUNT; ++i)
    {
        if ( m_buffLifeTimes[ i ] != BUFF_INFINITE )
            remove( i );
    }
}



// add time to a buff
void BuffComponent::AddBuffTime(BuffType type, const float lifeTime)
{
    if ( m_buffStatus[ type ] )
        m_buffLifeTimes[ type ] += lifeTime;
}


void BuffComponent::OnInitialize(void)
{
   // GetOwner( )->Listener( this )
   //     .On( game::, &BaseWeapon::TriggerReleased );

}




