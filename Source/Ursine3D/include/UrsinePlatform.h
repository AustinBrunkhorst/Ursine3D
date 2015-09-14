#pragma once

#ifdef _WIN32
#define PLATFORM_WINDOWS
#elif __unix__
#define PLATFORM_UNIX
#elif __linux__
#define PLATFORM_LINUX
#elif __APPLE__
#define PLATFORM_OSX
#endif