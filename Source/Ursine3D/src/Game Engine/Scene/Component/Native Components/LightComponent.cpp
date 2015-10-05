#include "UrsinePrecompiled.h"

#include "LightComponent.h"
#include "GfxAPI.h"

NATIVE_COMPONENT_DEFINITION(ursine::ecs::Light);

ursine::ecs::Light::Light( )
    : BaseComponent( )
    , m_light(nullptr)
    , m_handle(0)
{
}

ursine::ecs::Light::~Light()
{
    GetCoreSystem(graphics::GfxAPI)->RenderableMgr.DestroyRenderable(m_handle);
    m_light = nullptr;
}

void ursine::ecs::Light::OnInitialize()
{
    m_handle = GetCoreSystem(graphics::GfxAPI)->RenderableMgr.AddRenderable(graphics::RENDERABLE_LIGHT);
    m_light = &GetCoreSystem(graphics::GfxAPI)->RenderableMgr.GetLight(m_handle);
    m_light->Initialize( );
}

const ursine::graphics::GfxHND ursine::ecs::Light::GetHandle() const
{
    return m_handle;
}

const ursine::graphics::Light* ursine::ecs::Light::GetLight()
{
    return m_light;
}

ursine::graphics::Light::LightType ursine::ecs::Light::GetType()
{
    return m_light->GetType( );
}

void ursine::ecs::Light::SetType(const ursine::graphics::Light::LightType type)
{
    m_light->SetType(type);
}

const ursine::SVec3& ursine::ecs::Light::GetDirection()
{
    return m_light->GetDirection( );
}

void ursine::ecs::Light::SetDirection(const SVec3& dir)
{
    m_light->SetDirection(dir);
}

const ursine::SVec3& ursine::ecs::Light::GetPosition()
{
    return m_light->GetPosition( );
}

void ursine::ecs::Light::SetPosition(const SVec3& position)
{
    m_light->SetPosition(position);
}

const ursine::Color& ursine::ecs::Light::GetColor()
{
    return m_light->GetColor( );
}

void ursine::ecs::Light::SetColor(const Color& color)
{
    m_light->SetColor(color);
}

float ursine::ecs::Light::GetRadius()
{
    return m_light->GetRadius( );
}

void ursine::ecs::Light::SetRadius(const float radius)
{
    m_light->SetRadius(radius);
}

float ursine::ecs::Light::GetIntensity()
{
    return m_light->GetIntensity( );
}

void ursine::ecs::Light::SetIntensity(const float intensity)
{
    m_light->SetIntensity(intensity);
}

const ursine::Vec2& ursine::ecs::Light::GetSpotlightAngles()
{
    return m_light->GetSpotlightAngles( );
}

void ursine::ecs::Light::SetSpotlightAngles(const Vec2& angles)
{
    m_light->SetSpotlightAngles(angles);
}

void ursine::ecs::Light::SetWorldMatrix(const Mat4& worldTransform)
{
    //extract position data

    //extract radius data
}