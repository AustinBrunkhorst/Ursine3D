/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RigComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RigComponent.h"

#include "Notification.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Rig );

        Rig::Rig(void)
            : BaseComponent( ) { }

        Rig::~Rig(void)
        {
            // TODO: enable deletion on the rig entities that exist
        }

        const resources::ResourceReference &Rig::GetRig(void) const
        {
            return m_rig;
        }

        void Rig::SetRig(const resources::ResourceReference &rig)
        {
            m_rig = rig;

            if (!resourcesAreAvailable( ))
                return;

            // ask the user if they want to load the rig.
            // if no, revert the rig back

            NOTIFY_COMPONENT_CHANGED( "rig", m_rig );
        }

    #if defined(URSINE_WITH_EDITOR)

        void Rig::ImportRig(void)
        {
            if (m_rig.GetGUID( ) == kNullGUID)
                return;

            // If we currently have a generated rig root
            if (m_boneEntities.size( ) > 0)
            {
                auto rigRoot = m_boneEntities[ 0 ];

                NotificationConfig config;

                config.type = NOTIFY_WARNING;
                config.header = "Warning";
                config.message = "This action will delete all of the Rig_Root's children. Continue?";
                config.dismissible = false;
                config.duration = 0;

                NotificationButton yes, no;

                yes.text = "Yes";
                yes.onClick = [=](Notification &notification) {
                    notification.Close( );

                    // Main thread operation
                    Application::PostMainThread( [=] {
                        rigRoot->EnableDeletion( true );
                        rigRoot->Delete( );
                        importRig( );
                    } );
                };

                no.text = "No";
                no.onClick = [=](Notification &notification) {
                    notification.Close( );
                };

                config.buttons = { yes, no };

                EditorPostNotification( config );
                return;
            }

            Application::PostMainThread( [=] {
                importRig( );
            } );
        }

    #endif

        void Rig::importRig(void)
        {

        }
    }
}
