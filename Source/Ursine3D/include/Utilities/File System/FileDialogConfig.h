/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FileDialogConfig.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "FileSystem.h"
#include "Window.h"

namespace ursine
{
    namespace fs
    {
        enum FileDialogMode
        {
            FDM_OPEN,
            FDM_SAVE,
            FDM_DIRECTORY
        };

        struct FileDialogFilter
        {
            // Title of the filter
            std::string title;

            // Extensions supported in the filter
            std::vector<std::string> extensions;
        };

        struct FileDialogConfig
        {
            // Optional parent window for this dialog
            Window::Handle parentWindow { nullptr };

            // Title of the dialog that will be opened
            std::string windowTitle;

            // Type of dialog to be shown
            FileDialogMode mode { FDM_OPEN };

            // Enables selection of multiple files
            bool allowMultipleFiles { false };

            // Prompt the user if they will overwrite an existing file
            bool showOverwritePrompt { true };

            // Files that are readonly will not be listed
            bool hideReadOnlyFiles { false };

            // Initial path the dialog will be opened to
            // Empty just uses the last opened path
            path initialPath { "" };

            // List of filters to use in the dialog
            std::vector<FileDialogFilter> filters;

            // Index in "filters" to be selected by default
            unsigned selectedFilter { 0 };

            FileDialogConfig(void) = default;
        };

        struct FileDialogResult
        {
            // Index of the selected input filter that was selected
            unsigned selectedFilterIndex;

            // List of files that were selected
            PathList selectedFiles;

            operator bool(void)
            {
                return !selectedFiles.empty( );
            }
        };
    }
}