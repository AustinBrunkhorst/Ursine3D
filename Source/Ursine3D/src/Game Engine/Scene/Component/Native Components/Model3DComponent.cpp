#include "UrsinePrecompiled.h"

#include "Model3DComponent.h"
#include "RenderableComponent.h"

NATIVE_COMPONENT_DEFINITION( ursine::ecs::Model3D );

ursine::ecs::Model3D::Model3D()
    : BaseComponent( )
    , m_model(nullptr)
{
}

ursine::ecs::Model3D::~Model3D()
{
    
}

void ursine::ecs::Model3D::OnInitialize()
{
    
}