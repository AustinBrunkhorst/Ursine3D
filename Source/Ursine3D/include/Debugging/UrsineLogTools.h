/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** UrsineLogTools.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "DebuggingConfig.h"

#include "UrsineBuildConfig.h"
#include "UrsineTypes.h"
#include "UrsineConsoleColor.h"

#include "AssertionException.h"

#include <string>

#ifndef URSINE_OUTPUT_CONSOLE
    #ifdef CONFIG_DEBUG
        #define URSINE_OUTPUT_CONSOLE 1
    #else
        #define URSINE_OUTPUT_CONSOLE 0
    #endif
#endif

#ifndef URSINE_OUTPUT_FILE
    #define URSINE_OUTPUT_FILE 1
#endif

#ifndef URSINE_OUTPUT_WARNINGS
    #ifdef CONFIG_DEBUG
        #define URSINE_OUTPUT_WARNINGS 1
    #else
        #define URSINE_OUTPUT_WARNINGS 0
    #endif
#endif

#ifndef URSINE_DISPLAY_CRASH_MESSAGEBOX
    #ifdef CONFIG_DEBUG
        #define URSINE_DISPLAY_CRASH_MESSAGEBOX 0
    #else
        #define URSINE_DISPLAY_CRASH_MESSAGEBOX 1
    #endif
#endif

#ifndef URSINE_ERROR_LOG_FILE
    #ifdef CONFIG_DEBUG
        #define URSINE_ERROR_LOG_FILE "Debug.ErrorLog.txt"
    #else
        #define URSINE_ERROR_LOG_FILE "ErrorLog.txt"
    #endif
#endif

#ifndef URSINE_ASSERTION_LOG_HEADER
    #define URSINE_ASSERTION_LOG_HEADER "ASSERTION FAILED"
#endif

#ifndef URSINE_WARNING_LOG_HEADER
    #define URSINE_WARNING_LOG_HEADER "WARNING"
#endif

#ifndef URSINE_ERROR_LOG_HEADER
    #define URSINE_ERROR_LOG_HEADER "FATAL ERROR"
#endif

#ifndef URSINE_UNCAUGHT_ASSERTION_LOG_HEADER
    #define URSINE_UNCAUGHT_ASSERTION_LOG_HEADER "UNCAUGHT ASSERTION"
#endif

#if (URSINE_OUTPUT_CONSOLE | URSINE_OUTPUT_FILE)
    #define UAssert(assertion, ...) (!!(assertion) || (ursine::logging::Assert(URSINE_FFL,##__VA_ARGS__), 0))
    #define UError(message, ...) do { ursine::logging::Error(URSINE_FFL, message,##__VA_ARGS__); } while (0)

    #if (URSINE_OUTPUT_WARNINGS)
        #define UWarning(message, ...) do { ursine::logging::Warning(URSINE_FFL, message,##__VA_ARGS__); } while (0)
        #define UWarningIf(condition, message, ...) (!!!(condition) || (ursine::logging::Warning(URSINE_FFL, message,##__VA_ARGS__), 0))
    #else
        #define UWarning(message, ...)
        #define UWarningIf(condition, message, ...)
    #endif
#else
    #define UAssert(assertion, ...)
    #define UError(message, ...)
    #define UWarning(message, ...)
    #define UWarningIf(condition, message, ...)
#endif

namespace ursine
{
    namespace logging
    {
        extern int gExitCode;

        extern struct Initializer
        {
            Initializer(void);
        } gInitializer;

        template<typename... Args>
        void Assert(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED);

        template<typename... Args>
        void Warning(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED);

        template<typename... Args>
        void Error(URSINE_FFL_ARGS, URSINE_LOG_FORMATTED);

        void UncaughtAssertion(const AssertionException &exception);

        template<typename... Args>
        void Log(
            FILE *handle, 
            const std::string &title, 
            ConsoleColor titleColor, 
            URSINE_FFL_ARGS, 
            URSINE_LOG_FORMATTED
        );

        void OutputInfo(FILE *handle, URSINE_FFL_ARGS);

        void OutputTime(FILE *handle);
        void OutputStack(FILE *handle);
        void OutputWDir(FILE *handle);
        void OutputThreadInfo(FILE *handle);

        void ExitError(void);
    }
}

#include "UrsineLogTools.hpp"