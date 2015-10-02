#pragma once

#include <NativeJSClass.h>

class EntityHandler : public ursine::NativeJSClass
{
    JAVASCRIPT_CLASS;

public:
    JSConstructor(EntityHandler);

    JSMethod(IsValid);
    JSMethod(GetName);

private:
    ursine::ecs::EntityUniqueID m_handle;

    ursine::ecs::Entity *getEntity(void);  
};