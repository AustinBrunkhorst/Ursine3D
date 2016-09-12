/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** UIScreenData.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "UIScreenData.h"
#include "UIScreenReader.h"

namespace ursine
{
    namespace resources
    {
        UIScreenData::UIScreenData(const Json &data)
        {
            auto &projectData = data[ "project" ];
            auto &pathData = data[ "path" ];

            UAssertCatchable( projectData.is_string( ) && pathData.is_string( ),
                "Invalid JSON screen structure."    
            );

            m_project = GUIDStringGenerator( )( projectData.string_value( ) );
            m_path = pathData.string_value( );
            m_name = m_path.stem( ).string( );
        }

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
            return typeof( UIScreenReader );
        }
    }
}