#include "UrsinePrecompiled.h"

#include "UIResourceData.h"
#include "UIResourceReader.h"

namespace ursine
{
    namespace resources
    {
        UIResourceData::UIResourceData(ResourceTable &&table)
            : m_table( std::move( table ) ) { }

        BinaryData::Handle UIResourceData::GetEntry(const fs::path &entryPath) const
        {
            auto search = m_table.find( entryPath );

            if (search == m_table.end( ))
                return nullptr;

            return search->second;
        }

        void UIResourceData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( static_cast<unsigned>( m_table.size( ) ) );

            for (auto &entry : m_table)
            {
                output.Write( entry.first.string( ) );
                output.Write( *entry.second );
            }
        }

        meta::Type UIResourceData::GetReaderType(void)
        {
            return typeof( UIResourceReader );
        }
    }
}