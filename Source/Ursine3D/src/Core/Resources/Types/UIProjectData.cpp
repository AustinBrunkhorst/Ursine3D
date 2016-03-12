#include "UrsinePrecompiled.h"

#include "UIProjectData.h"
#include "UIProjectReader.h"

namespace ursine
{
    namespace resources
    {
        UIProjectData::UIProjectData(ResourceTable &&table)
            : m_table( std::move( table ) ) { }

        BinaryData::Handle UIProjectData::GetEntry(const fs::path &entryPath) const
        {
            auto search = m_table.find( entryPath );

            if (search == m_table.end( ))
                return nullptr;

            return search->second;
        }

        void UIProjectData::Write(pipeline::ResourceWriter &output)
        {
            output.Write( static_cast<unsigned>( m_table.size( ) ) );

            for (auto &entry : m_table)
            {
                output.Write( entry.first.string( ) );
                output.Write( *entry.second );
            }
        }

        meta::Type UIProjectData::GetReaderType(void)
        {
            return typeof( UIProjectReader );
        }
    }
}