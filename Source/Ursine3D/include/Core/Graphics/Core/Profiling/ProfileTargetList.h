/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      ProfileTargetList.h
Module:         Graphics
Purpose:        Enum of profiling targets
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

        //this MUST be in the order they are called
        enum ProfilingTargets
        {
            PROFILE_FRAME_BEGIN = 0,
            //leave above as first

            PROFILE_CLEAR_BUFFERS,
            PROFILE_SORT,
            PROFILE_DEFERRED, //big (2nd)

#if defined(URSINE_WITH_EDITOR)
            PROFILE_COMPUTEMOUSE,
#endif

            PROFILE_LIGHTS, //big (1st)
            PROFILE_PRIMITIVES,
            PROFILE_DEBUG,
            PROFILE_SCENE_MAIN, //big (3rd)
            PROFILE_SCENE_PRIMITIVE,


            //leave as last
            //PROFILE_FRAME_END,
            PROFILE_COUNT
        };
    }
}