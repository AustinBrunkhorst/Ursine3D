#include "UrsinePrecompiled.h"

#include "PointLightComponent.h"
#include "RenderableComponent.h"

NATIVE_COMPONENT_DEFINITION( ursine::ecs::PointLight );

ursine::ecs::PointLight::PointLight()
    : BaseComponent( )
    , m_pointLight( nullptr )
{
}

ursine::ecs::PointLight::~PointLight()
{
    
}

void ursine::ecs::PointLight::SetColor(const Color& color) {}

void ursine::ecs::PointLight::SetColor(const float& r, const float& g, const float& b) {}

void ursine::ecs::PointLight::SetPosition(const SVec3& position) {}

void ursine::ecs::PointLight::SetPosition(const float& x, const float& y, const float& z) {}

void ursine::ecs::PointLight::SetRadius(const float& radius) {}

void ursine::ecs::PointLight::OnInitialize()
{
    
}