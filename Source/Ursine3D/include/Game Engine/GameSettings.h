/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GameSettings.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    struct GameSettings : meta::Object
    {
        META_OBJECT;

        std::string windowTitle;
        bool windowFullScreen;
        bool windowResizable;

        std::string manager;

        Meta(Disable)
        Json managerConfiguration;

        Meta(Enable)
        GameSettings(void) = default;

        void OnSerialize(Json::object &output) const override;
        void OnDeserialize(const Json &input) override;
    } Meta(WhiteListMethods);
}