#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ForceComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <BoxColliderComponent.h>
#include <SVec3.h>

struct ForceComponent : ursine::ecs::Component
{
    NATIVE_COMPONENT
public:

    ForceComponent(void);
    ~ForceComponent(void);

    void OnInitialize(void) override;

    EditorField(
        ursine::SVec3 Force,
        GetForce,
        SetForce
        );
    const ursine::SVec3& GetForce(void) const;
    void SetForce(const ursine::SVec3& force);
    
    void OnCollide(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));

    ursine::SVec3 m_force;

} Meta(Enable, DisplayName("ForceComponent"), RequiresComponents(typeof(ursine::ecs::BoxCollider)));

