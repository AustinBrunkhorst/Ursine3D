/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DebuggingConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#define URSINE_FFL __FILE__, __FUNCTION__, __LINE__

#define URSINE_FFL_ARGS const std::string &file,    \
                        const std::string &function,\
                        uint line                   \

#define URSINE_LOG_FORMATTED const std::string format,\
                             const Args&... args      \

