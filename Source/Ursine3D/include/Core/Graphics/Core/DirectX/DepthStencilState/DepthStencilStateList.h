/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DepthStencilStateList.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DepthStencilList.h
Module:         Graphics
Purpose:        enum of possible depth states
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace graphics
    {
        enum DEPTH_STATES
        {
            DEPTH_STATE_DEPTH_NOSTENCIL = 0,
            DEPTH_STATE_NODEPTH_NOSTENCIL,

            DEPTH_STATE_DEPTH_STENCIL,
            DEPTH_STATE_NODEPTH_STENCIL,

            DEPTH_STATE_POINTLIGHT,
            DEPTH_STATE_PASSDEPTH_WRITESTENCIL,

            DEPTH_STATE_PASSDEPTH_NOSTENCIL,

            DEPTH_STATE_CHECKDEPTH_NOWRITE_NOSTENCIL,

            DEPTH_STATE_COUNT
        };
    }
}
