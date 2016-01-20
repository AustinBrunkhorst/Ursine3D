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

#include "Precompiled.h"

#include "SelectedComponent.h"

#include <Model3DComponent.h>

using namespace ursine;

NATIVE_COMPONENT_DEFINITION( Selected );

Selected::Selected(void)
    : BaseComponent( ) 
    , m_emissive( 0 ) { }

Selected::~Selected(void)
{
    tryDebugModel( false );
}

void Selected::OnInitialize(void)
{
    Component::OnInitialize( );

    auto owner = GetOwner( );

    tryDebugModel( true );
}

void Selected::tryDebugModel(bool enabled)
{
    auto model = GetOwner( )->GetComponent<ecs::Model3D>( );

    // nope the fuck out of here
    if (!model)
        return;

    model->SetDebug( enabled );

    float e, p, i;
    model->GetMaterialData(e, p, i);

    //save the emissive?
    if (enabled)
        m_emissive = e;

    URSINE_TODO( "This should probably be removed" );
    if (enabled)
        model->SetMaterialData( 1, 0, 0 );
    else
        model->SetMaterialData(m_emissive, 0, 0 );
}