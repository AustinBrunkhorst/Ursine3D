/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SerializationConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <exception>

namespace ursine
{
    namespace ecs
    {
        const auto kSerializationVersion = "0.2submission";

        class SerializationException
        {
        public:
            SerializationException(const std::string &error);

            const std::string &GetError(void) const;

        private:
            std::string m_error;
        };
    }
}