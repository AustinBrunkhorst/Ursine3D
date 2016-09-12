/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CritSpotComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "CritSpotComponent.h"
#include <GameEvents.h>

NATIVE_COMPONENT_DEFINITION( CritSpot ) ;


CritSpot::CritSpot(void) :
    BaseComponent( ),
    m_search( false )
{
}

CritSpot::~CritSpot(void)
{
}

bool CritSpot::GetSearch(void) const
{
    return m_search;
}

void CritSpot::SetSearch(bool search)
{
    m_search = search;
}


void CritSpot::OnInitialize(void)
{
}


