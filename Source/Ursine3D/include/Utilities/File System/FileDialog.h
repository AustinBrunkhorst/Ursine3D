/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FileDialog.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "FileSystem.h"
#include "FileDialogConfig.h"

namespace ursine
{
    namespace fs
    {
        class FileDialog
        {
        public:
            FileDialogConfig config;

            FileDialogResult Open(void);

        private:
            static FileDialogResult platformImplOpen(const FileDialogConfig &config);
        };
    }
}