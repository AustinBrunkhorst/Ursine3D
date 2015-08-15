/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Asset.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <string>

namespace Ursine
{
    class Asset
    {
    protected:
        const std::string _path;

    public:
        Asset(const std::string &path) 
            : _path(path) {}

        virtual ~Asset(void) {}

        inline const std::string &GetPath(void) const;
    };
}

#include "Asset.hpp"