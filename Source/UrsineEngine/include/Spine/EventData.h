#pragma once

namespace ursine
{
    namespace spine
    {
        struct EventData
        {
            const std::string name;

            int int_value;
            float float_value;
            std::string string_value;

            EventData(const char *name);
            ~EventData(void);
        };
    }
}
