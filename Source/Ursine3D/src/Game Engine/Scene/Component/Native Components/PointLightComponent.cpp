#include "UrsinePrecompiled.h"

#include "PointLightComponent.h"
#include "RenderableComponent.h"

#include "GfxAPI.h"
#include "EntityManager.h"
#include "Entity.h"

NATIVE_COMPONENT_DEFINITION( ursine::ecs::PointLight );

ursine::ecs::PointLight::PointLight(void)
    : BaseComponent( )
    , m_pointLight( nullptr )
{
    
}

ursine::ecs::PointLight::~PointLight(void)
{
    URSINE_TODO( "..." );
    //GetCoreSystem( ursine::GfxAPI )->RenderableMgr.DestroyRenderable( m_handle );
}

void ursine::ecs::PointLight::OnInitialize(void)
{
    auto *renderable = GetOwner( )->GetComponent<Renderable>( );

    auto handle = GetCoreSystem( ursine::graphics::GfxAPI )->RenderableMgr.AddRenderable(graphics::RENDERABLE_POINT_LIGHT );

    m_pointLight = &GetCoreSystem( ursine::graphics::GfxAPI )->RenderableMgr.GetPointLight( handle );

    renderable->SetHandle( handle );
}

void ursine::ecs::PointLight::SetPosition(const SVec3 &position)
{
    m_pointLight->SetPosition( position );
}

float ursine::ecs::PointLight::GetRadius(void) const
{
    return m_pointLight->GetRadius( );
}

void ursine::ecs::PointLight::SetRadius(float radius)
{
    m_pointLight->SetRadius( radius );
}

const ursine::Color &ursine::ecs::PointLight::GetColor(void) const
{
    return m_pointLight->GetColor( );
}

void ursine::ecs::PointLight::SetColor(const Color &color)
{
    m_pointLight->SetColor( color );
}