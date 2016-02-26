/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerIdCompnent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "PlayerIdComponent.h"

NATIVE_COMPONENT_DEFINITION( PlayerID );


PlayerID::PlayerID( void )
    : BaseComponent( )
    , m_id( 0 )
{
}

int PlayerID::GetID( void ) const
{
    return m_id;
}

void PlayerID::SetID(const int id)
{
    m_id = id;
}

