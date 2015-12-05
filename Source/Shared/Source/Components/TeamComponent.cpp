#include "Precompiled.h"

#include"TeamComponent.h"

NATIVE_COMPONENT_DEFINITION(TeamComponent);

using namespace ursine;

TeamComponent::TeamComponent(): BaseComponent()
                              , m_teamNumber(0)
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