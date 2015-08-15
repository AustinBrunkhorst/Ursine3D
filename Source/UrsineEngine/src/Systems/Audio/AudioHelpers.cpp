/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioHelpers.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "AudioHelpers.h"

namespace Ursine
{
    void FMODERRCHK(FMOD_RESULT result)
    {
        if (result != FMOD_OK)
        {
            printf("FMOD error! (%d)  %s\n", result, FMOD_ErrorString(result));
        }
    }
}