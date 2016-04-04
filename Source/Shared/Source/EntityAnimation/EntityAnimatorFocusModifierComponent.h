/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorFocusModifierComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "EntityAnimatorComponent.h"

class EntityAnimatorFocusModifier : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string focusEntity,
        GetFocusEntity,
        SetFocusEntity
    );

    EntityAnimatorFocusModifier(void);
    ~EntityAnimatorFocusModifier(void);

    const std::string &GetFocusEntity(void) const;
    void SetFocusEntity(const std::string &entity);

private:

    std::string m_focusEntity;

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents( typeof( EntityAnimator ) )
);
