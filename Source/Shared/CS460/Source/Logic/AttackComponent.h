/*---------------------------------------------------------------------------
**Team Bear King
**?2015 DigiPen Institute of Technology, All Rights Reserved.
**
**AttackComponent.h
**
**Author:
**- Jordan Ellis - j.ellis@digipen.edu
**
**Contributors:
**- <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include "IKRootComponent.h"

class Attack : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    Attack(void);

    void OnInitialize(void) override;

private:
    bool m_attacking;
    float m_time;
    float m_t;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

} Meta(Enable) EditorMeta(RequiresComponents(typeof(IKRoot)));
