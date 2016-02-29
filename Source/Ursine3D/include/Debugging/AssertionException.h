#pragma once

#include "DebuggingConfig.h"

#include <string>

#define UAssertCatchable(assertion, ...) if(!(assertion)) { throw ursine::AssertionException(URSINE_FFL,##__VA_ARGS__); }

namespace ursine
{
    class AssertionException
    {
    public:
        AssertionException(void);

        template<typename... Args>
        AssertionException(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED);

        const std::string &GetFile(void) const;
        const std::string &GetFunction(void) const;

        uint GetLine(void) const;

        const std::string &GetErrorMessage(void) const;

    private:
        std::string m_file;
        std::string m_function;
        uint m_line;
        std::string m_message;
    };
}

#include "AssertionException.hpp"
