/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SegmentLogicState.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"

class TutorialResources : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string simulationParticleArchetype,
        GetSimulationParticleArchetype,
        SetSimulationParticleArchetype
    );

    TutorialResources(void);

    const std::string &GetSimulationParticleArchetype(void) const;
    void SetSimulationParticleArchetype(const std::string &name);

private:

    std::string m_simulationParticle;

} Meta(Enable);
