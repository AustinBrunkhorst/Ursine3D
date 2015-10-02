 #include "UrsinePrecompiled.h"

#include "DirectionalLightComponent.h"
#include "RenderableComponent.h"

NATIVE_COMPONENT_DEFINITION( ursine::ecs::DirectionalLight );

ursine::ecs::DirectionalLight::DirectionalLight()
    : BaseComponent( )
    , m_directionalLight( nullptr )
{
}

ursine::ecs::DirectionalLight::~DirectionalLight()
{
    
}

void ursine::ecs::DirectionalLight::OnInitialize()
{
    //grab the handle from the gfx manager

    //save a ptr to the object
}

const ursine::SVec3& ursine::ecs::DirectionalLight::GetDirection() const
{
    return m_directionalLight->GetDirection( );
}

void ursine::ecs::DirectionalLight::SetDirection(const SVec3& dir)
{
    m_directionalLight->SetDirection( dir );
}

void ursine::ecs::DirectionalLight::SetDirection(const float& x, const float& y, const float& z)
{
    m_directionalLight->SetDirection( x, y, z );
}

const ursine::Color& ursine::ecs::DirectionalLight::GetColor() const
{
    return m_directionalLight->GetColor( );
}

void ursine::ecs::DirectionalLight::SetColor(const Color& color)
{
    m_directionalLight->SetColor( color );
}

void ursine::ecs::DirectionalLight::SetColor(const float r, const float g, const float b)
{
    m_directionalLight->SetColor( r, g, b );
}