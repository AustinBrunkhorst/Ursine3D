/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ISerialize.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <windows.h>
#include "ResourceReader.h"
#include "ResourceWriter.h"

#define MAXTEXTLEN 256

namespace ursine
{
    namespace graphics
    {
        namespace ufmt_loader
        {
            class ISerialize
            {
            public:
                /** @brief serializer constructor
                *
                *  this will construct basic serializer
                *
                *  @return nothing
                */
                ISerialize();
                /** @brief serializer destructor
                *
                *  this will destroy basic serializer
                *
                *  @return nothing
                */
                virtual ~ISerialize();

                virtual void Read(resources::ResourceReader &input) = 0;
                virtual void Write(resources::pipeline::ResourceWriter &output) = 0;
            };
        };
    };
};