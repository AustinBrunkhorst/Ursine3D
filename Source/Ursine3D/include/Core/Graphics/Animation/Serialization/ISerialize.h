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
            private:
                char* m_FileInfoName;
            protected:
            public:
                /** @brief serializer constructor
                *
                *  this will construct basic serializer
                *
                *  @return nothing
                */
                ISerialize(const char* FileInfoName = "");
                /** @brief serializer destructor
                *
                *  this will destroy basic serializer
                *
                *  @return nothing
                */
                virtual ~ISerialize();

                /** @brief serialize in function interface
                *
                *  this will generate basic interface for the serializer
                *
                *  @param handle of the file
                *  @return if succeed return true, else return false
                */
                virtual bool SerializeIn(HANDLE hFile) = 0;
                /** @brief serialize out function interface
                *
                *  this will generate basic interface for the serializer
                *
                *  @param handle of the file
                *  @return if succeed return true, else return false
                */
                virtual bool SerializeOut(HANDLE hFile) = 0;

                virtual void SerializeIn(resources::ResourceReader &input) = 0;
                virtual void SerializeOut(resources::pipeline::ResourceWriter &output) = 0;
            };
        };
    };
};