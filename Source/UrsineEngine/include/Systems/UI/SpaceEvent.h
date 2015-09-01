#pragma once

#include "EventArgs.h"
#include "Json.h"

#include <string>

namespace ursine
{
    namespace UI
    {
        struct SpaceMessageArgs : public EventArgs
        {
            const std::string &name;
            const Json &data;

            SpaceMessageArgs(const std::string &name, const Json &data)
                : name(name)
                , data(data) {}
        };
    }
}