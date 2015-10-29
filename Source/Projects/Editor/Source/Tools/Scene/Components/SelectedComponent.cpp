#include "Precompiled.h"

#include "SelectedComponent.h"

#include <RenderableComponent.h>
#include <Model3DComponent.h>

using namespace ursine;

NATIVE_COMPONENT_DEFINITION( Selected );

Selected::Selected(void)
    : BaseComponent( ) 
    , m_selectBox( nullptr ) { }

Selected::~Selected(void)
{
    m_selectBox->Delete( );

    tryDebugModel( false );
}

void Selected::OnInitialize(void)
{
    auto owner = GetOwner( );

    m_selectBox = owner->GetWorld( )->CreateEntity( "DebugBox" );

    m_selectBox->SetVisibleInEditor( false );

    auto *renderable = m_selectBox->AddComponent<ecs::Renderable>( );

    auto &renderableManager = GetCoreSystem( graphics::GfxAPI )->RenderableMgr;

    auto handle = renderableManager.AddRenderable( graphics::RENDERABLE_PRIMITIVE );
    auto &prim = renderableManager.GetPrimitive( handle );

    auto transform = owner->GetTransform( );

    prim.SetType( graphics::Primitive::PRIM_CUBE );

    prim.SetWorldMatrix( transform->GetLocalToWorldMatrix( ) * SMat4( 1.1, 1.1, 1.1 ) );
    
    renderable->SetHandle( handle );

    tryDebugModel( true );
}

void Selected::tryDebugModel(bool enabled)
{
    auto model = GetOwner( )->GetComponent<ecs::Model3D>( );

    // nope the fuck out of here
    if (!model)
        return;

    auto *handle = model->GetModel( );

    handle->SetDebug( enabled );

    URSINE_TODO( "This should probably be removed" );
    if (enabled)
        handle->SetMaterialData( 1, 0, 0 );
    else
        handle->SetMaterialData( 0, 0, 0 );
}