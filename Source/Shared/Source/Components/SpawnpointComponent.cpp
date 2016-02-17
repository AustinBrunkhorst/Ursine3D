/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnpointComponent.cpp
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include"SpawnpointComponent.h"

NATIVE_COMPONENT_DEFINITION(Spawnpoint);

using namespace ursine;

Spawnpoint::Spawnpoint(): BaseComponent()
                        , m_teamNumber(-1)
                        , m_roundSpawnNumber(0)
{

}

int Spawnpoint::GetTeamNumber() const
{
    return m_teamNumber;
}

void Spawnpoint::SetTeamNumber(int teamNo)
{
    m_teamNumber = teamNo;
}

int Spawnpoint::GetRoundSpawnNumber() const
{
    return m_roundSpawnNumber;
}

float Spawnpoint::GetYRotationDegrees(void) const
{
    return m_yRotationDegrees;
}

void Spawnpoint::SetYRotationDegrees(float degrees)
{
    m_yRotationDegrees = degrees;
}

void Spawnpoint::SetRoundSpawnNumber(int spawnNo)
{
    m_roundSpawnNumber = spawnNo;
}