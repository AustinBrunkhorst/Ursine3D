#pragma once

namespace ursine
{
    namespace resources
    {
        class ResourceReader : public meta::Object
        {
        public:
            template<typename T>
            ResourceReader &operator>>(T &output);

        private:
            std::ifstream m_stream;
        };
    }
}

#include "ResourceReader.hpp"