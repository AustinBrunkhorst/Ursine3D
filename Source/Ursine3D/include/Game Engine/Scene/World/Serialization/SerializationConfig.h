#pragma once

#include <exception>

namespace ursine
{
    namespace ecs
    {
        const auto kSerializationVersion = "0.0";

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