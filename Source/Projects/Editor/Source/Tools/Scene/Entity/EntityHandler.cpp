#include "Precompiled.h"

#include "EntityHandler.h"

JSConstructor(EntityHandler)
    : m_handle( 0 )
{
        
}

JSMethod(EntityHandler::GetName)
{
    return CefV8Value::CreateString( m_handle->GetName( ) );
}

ursine::ecs::Entity *EntityHandler::getEntity(void)
{
    
}