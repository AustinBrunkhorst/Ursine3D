/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Space.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/


#include "UrsinePrecompiled.h"

#include "Space.h"
#include "SpaceConfig.h"

#include "UISystem.h"

namespace ursine
{
    namespace UI
    {
        Space::Space(const std::string &name)
            : EventDispatcher(this)
            , _entered(false)
            , _owner(0)
            , _name(name)
        {
            
        }

        void Space::Entered(const Json &data)
        {
            UAssert(gUISystem, "UI System not created.");

            Json::object config {
                { "name", _name },
                { "owner", static_cast<int>(_owner) },
                { "data", data }
            };

            gUISystem->Message(kSpaceEventEntered, config);
        }

        void Space::Exited(void)
        {
            UAssert(gUISystem, "UI System not created.");

            Json::object data {
                { "owner", static_cast<int>(_owner) }
            };

            gUISystem->Message(kSpaceEventExited, data);
        }

        const std::string &Space::GetName(void) const
        {
            return _name;
        }

        void Space::Message(const std::string &name, const Json &data) const
        {
            Json::object config {
                { "owner", static_cast<int>(_owner) },
                { "name", name },
                { "data", data.dump() }
            };

            gUISystem->Message(kSpaceEventMessaged, config);
        }
    }
}