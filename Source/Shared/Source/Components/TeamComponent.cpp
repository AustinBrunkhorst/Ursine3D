/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TeamComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include"TeamComponent.h"

NATIVE_COMPONENT_DEFINITION(TeamComponent);

using namespace ursine;

TeamComponent::TeamComponent()
	: BaseComponent()
    , m_teamNumber(0)
	, m_alive(true)
{
}

int TeamComponent::GetTeamNumber() const
{
    return m_teamNumber;
}

void TeamComponent::SetTeamNumber(int num)
{
    m_teamNumber = num;
}

void TeamComponent::SetAlive(void)
{
	m_alive = true;
}

void TeamComponent::SetDead(void)
{
	m_alive = false;
}

bool TeamComponent::IsDead(void)
{
	return !m_alive;
}