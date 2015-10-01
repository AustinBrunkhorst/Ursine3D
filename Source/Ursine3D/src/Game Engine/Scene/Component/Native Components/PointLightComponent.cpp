#include "UrsinePrecompiled.h"

#include "PointLightComponent.h"
#include "RenderableComponent.h"

ursine::ecs::PointLight::PointLight()
    : BaseComponent( )
    , m_pointLight( nullptr )
{
}

ursine::ecs::PointLight::~PointLight()
{
    
}

void ursine::ecs::PointLight::OnInitialize()
{
    
}