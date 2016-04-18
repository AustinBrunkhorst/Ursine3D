/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GameSettings.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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