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

void Spawnpoint::SetRoundSpawnNumber(int spawnNo)
{
    m_roundSpawnNumber = spawnNo;
}