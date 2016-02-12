#pragma once

#include "ResourceData.h"

#include <memory>

namespace ursine
{
    namespace resources
    {
        typedef std::shared_ptr<ResourceData> ResourceDataHandle;
    }
}