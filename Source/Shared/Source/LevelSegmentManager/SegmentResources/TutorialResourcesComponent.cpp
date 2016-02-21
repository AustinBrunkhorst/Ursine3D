
#include "Precompiled.h"

#include "TutorialResourcesComponent.h"

NATIVE_COMPONENT_DEFINITION( TutorialResources );

TutorialResources::TutorialResources(void)
    : BaseComponent( )
{
}

const std::string &TutorialResources::GetSimulationParticleArchetype(void) const
{
    return m_simulationParticle;    
}

void TutorialResources::SetSimulationParticleArchetype(const std::string &name)
{
    m_simulationParticle = name;
}
