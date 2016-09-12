/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FileDialog.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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