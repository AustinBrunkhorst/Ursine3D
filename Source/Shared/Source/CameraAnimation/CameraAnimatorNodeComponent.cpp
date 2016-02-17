/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAnimatorNodeComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "CameraAnimatorNodeComponent.h"

NATIVE_COMPONENT_DEFINITION( CameraAnimatorNode );

CameraAnimatorNode::CameraAnimatorNode(void)
    : BaseComponent( )
    , m_transitionSeconds( 0.0f )
    , m_order( 0 )
{

}

float CameraAnimatorNode::GetTransitionToTime(void) const
{
    return m_transitionSeconds;
}

int CameraAnimatorNode::GetOrder(void) const
{
    return m_order;
}

void CameraAnimatorNode::SetOrder(int order)
{
    m_order = order;
}

void CameraAnimatorNode::SetTransitionToTime(float seconds)
{
    m_transitionSeconds = seconds;
}
