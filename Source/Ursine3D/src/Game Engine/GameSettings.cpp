#include "UrsinePrecompiled.h"

#include "GameSettings.h"

namespace ursine
{
    void GameSettings::OnSerialize(Json::object &output) const
    {
        output[ "managerSettings" ] = managerConfiguration;
    }

    void GameSettings::OnDeserialize(const Json &input)
    {
        managerConfiguration = input[ "managerSettings" ];
    }
}