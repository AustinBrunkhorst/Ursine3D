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

            output.WriteString( jsonString );
        }

        meta::Type JsonData::GetReaderType(void)
        {
            return typeof( JsonReader );
        }
    }
}