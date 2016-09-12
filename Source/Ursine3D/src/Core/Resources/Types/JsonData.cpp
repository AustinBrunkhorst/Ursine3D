/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** JsonData.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "JsonData.h"
#include "JsonReader.h"

namespace ursine
{
    namespace resources
    {
        JsonData::JsonData(const Json &data)
            : m_data( data ) { }

        const Json &JsonData::GetData(void) const
        {
            return m_data;
        }

        void JsonData::Write(pipeline::ResourceWriter &output)
        {
            auto jsonString = m_data.dump( false );

            output.Write( jsonString );
        }

        meta::Type JsonData::GetReaderType(void)
        {
            return typeof( JsonReader );
        }
    }
}