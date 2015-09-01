/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AudioHelpers.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>
#include <stdio.h>

namespace ursine
{
    namespace FMOD_S = FMOD::Studio;

    void FMODERRCHK(FMOD_RESULT result);
}