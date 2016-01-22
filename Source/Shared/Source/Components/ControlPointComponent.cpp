/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ControlPointComponent.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>
#include "ControlPointComponent.h"

NATIVE_COMPONENT_DEFINITION( ControlPoint ) ;


ControlPoint::ControlPoint( void )
    : BaseComponent( )
    , m_controlTimer( 10.0f )
{
}

float ControlPoint::GetControlTimer( void ) const
{
    return m_controlTimer;
}

void ControlPoint::SetControlTimer(const float time)
{
    m_controlTimer = time;
}
