/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ResourceData.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#define RESOURCE_DATA(dataType) META_OBJECT \
public: typedef std::shared_ptr<dataType> Handle; private: \


#include "ResourceWriter.h"

namespace ursine
{
    namespace resources
    {
        namespace pipeline
        {
            class ResourceWriter;
        }

        class ResourceData : public meta::Object
        {
        public:
            typedef std::shared_ptr<ResourceData> Handle;
            
            // Writes this resource instance to its built form
            virtual void Write(pipeline::ResourceWriter &output) = 0;

            // Type of runtime reader to use when loading built content
            virtual meta::Type GetReaderType(void) = 0;
        } Meta(WhiteListMethods);
    }
}