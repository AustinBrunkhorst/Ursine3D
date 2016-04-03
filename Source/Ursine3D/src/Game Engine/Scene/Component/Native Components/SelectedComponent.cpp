/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SelectedComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#if defined(URSINE_WITH_EDITOR)

#include "SelectedComponent.h"
#include "Model3DComponent.h"
#include "CameraComponent.h"
#include "WorldConfigComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Selected );

        Selected::Selected(void)
            : BaseComponent( ) { }

        Selected::~Selected(void)
        {
            tryDebugModel( false );
            tryDebugCamera( false );

	        auto owner = GetOwner( );

	        owner->GetWorld( )->Listener( this )
		        .Off( WORLD_ENTITY_COMPONENT_ADDED, &Selected::onComponentAdded );
        }

        void Selected::OnInitialize(void)
        {
            auto owner = GetOwner( );

            auto world = owner->GetWorld( );

	        world->Listener( this )
		        .On( WORLD_ENTITY_COMPONENT_ADDED, &Selected::onComponentAdded );

            tryDebugModel( true );
            tryDebugCamera( true );
        }

        void Selected::tryDebugModel(bool enabled)
        {
            auto model = GetOwner( )->GetComponent<Model3D>( );

            // nope the fuck out of here
            if (!model)
                return;

            model->SetDebug( enabled );
        }

        void Selected::tryDebugCamera(bool enabled)
        {
            auto camera = GetOwner( )->GetComponent<Camera>( );

            // nope the fuck out of here
            if (!camera)
                return;

            camera->SetEditorSelectionMode( enabled );
        }

        void Selected::onComponentAdded(EVENT_HANDLER(ecs::World))
        {
	        EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);
	
	        if (args->component->Is<ecs::Model3D>( ))
		        tryDebugModel( true );
        }
    }
}

#endif // defined(URSINE_WITH_EDITOR)
