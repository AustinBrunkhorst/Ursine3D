#include "UrsinePrecompiled.h"

#include "FileDialog.h"

namespace ursine
{
    namespace fs
    {
        FileDialogResult FileDialog::Open(void)
        {
            return platformImplOpen( config );
        }
    }
}