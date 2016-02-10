#include "UrsinePrecompiled.h"

#include "WorldConfigComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( WorldConfig );

        WorldConfig::WorldConfig(void)
            : BaseComponent( )
            , m_inEditorMode( false )
        {
            
        }

        WorldConfig::~WorldConfig(void)
        {
            
        }

        bool WorldConfig::IsInEditorMode(void) const
        {
            return m_inEditorMode;
        }

        void WorldConfig::SetInEditorMode(bool inEditorMode)
        {
            m_inEditorMode = inEditorMode;
        }

        const WorldSystemArray &WorldConfig::GetSystems(void) const
        {
            return m_systems;
        }

        void WorldConfig::SetSystems(const WorldSystemArray &systems)
        {
            m_systems = systems;

            NOTIFY_COMPONENT_CHANGED( "systems", systems );
        }

        void WorldConfig::OnInitialize(void)
        {
            Component::OnInitialize( );
        }
    }
}