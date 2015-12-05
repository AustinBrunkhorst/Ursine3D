#include "Precompiled.h"

#include"TeamComponent.h"

NATIVE_COMPONENT_DEFINITION(TeamComponent);

using namespace ursine;

TeamComponent::TeamComponent(): BaseComponent()
                              , TeamNumber(0)
{
}