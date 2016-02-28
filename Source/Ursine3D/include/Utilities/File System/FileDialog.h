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