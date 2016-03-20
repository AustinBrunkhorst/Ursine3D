#include "UrsinePrecompiled.h"

#include "UIScreenData.h"

#include "JsonReader.h"

namespace ursine
{
    namespace resources
    {
        UIScreenData::UIScreenData(const GUID &project, const fs::path &path)
            : m_project( project )
            , m_path( path )
            , m_name( path.stem( ).string( ) ) { }

        const GUID &UIScreenData::GetProject(void) const
        {
            return m_project;
        }

        const fs::path &UIScreenData::GetPath(void) const
        {
            return m_path;
        }

        const std::string &UIScreenData::GetName(void) const
        {
            return m_name;
        }

        fs::path UIScreenData::GetQualifiedPath(void) const
        {
            return to_string( m_project ) / m_path;
        }

        void UIScreenData::Write(pipeline::ResourceWriter &output)
        {
            Json screenData = Json::object {
                { "project", to_string( m_project ) },
                { "path", m_path.string( ) }
            };

            output.Write( screenData.dump( false ) );
        }

        meta::Type UIScreenData::GetReaderType(void)
        {
            return typeof( JsonReader );
        }
    }
}