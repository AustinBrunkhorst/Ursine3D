#pragma once

#include <NativeJSClass.h>

class EntityHandler : public ursine::NativeJSClass
{
    JAVASCRIPT_CLASS;

public:
    JSConstructor(EntityHandler);

    JSMethod(isValid);

    JSMethod(getName);
    JSMethod(setName);
    
    JSMethod(inspect);

    JSMethod(hasComponent);
    JSMethod(addComponent);
    JSMethod(removeComponent);

    JSMethod(updateComponentField);

private:
    ursine::ecs::World *m_world;
    ursine::ecs::EntityUniqueID m_handle;

    ursine::ecs::Entity *getEntity(void);  
} Meta(Enable);