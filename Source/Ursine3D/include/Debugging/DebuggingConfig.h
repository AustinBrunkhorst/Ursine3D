#pragma once

#define URSINE_FFL __FILE__, __FUNCTION__, __LINE__

#define URSINE_FFL_ARGS const std::string &file,    \
                        const std::string &function,\
                        uint line                   \

#define URSINE_LOG_FORMATTED const std::string format,\
                             const Args&... args      \

