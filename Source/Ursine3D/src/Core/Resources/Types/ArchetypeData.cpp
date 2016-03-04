#include "UrsinePrecompiled.h"

#include "ArchetypeData.h"
#include "ArchetypeReader.h"

namespace ursine
{
    namespace resources
    {
        ArchetypeData::ArchetypeData(const Json &archetypeJson)
            : m_data( archetypeJson ) { }

        const Json &ArchetypeData::GetData(void)
        {
            return m_data;
        }

        void ArchetypeData::Write(pipeline::ResourceWriter &output)
        {
            auto jsonString = m_data.dump( false );

            output.WriteString( jsonString );
        }

        meta::Type ArchetypeData::GetReaderType(void)
        {
            return typeof( ArchetypeReader );
        }
    }
}