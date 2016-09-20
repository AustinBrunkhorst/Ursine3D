/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LightingGroupComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class LightingGroup : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        bool initiallyActive,
        GetInitiallyActive,
        SetInitiallyActive
    );

    LightingGroup(void);

    void OnSceneReady(ursine::Scene *scene) override;

    bool GetInitiallyActive(void) const;
    void SetInitiallyActive(bool active);

    void ToggleGroupLights(bool toggle);

private:

    bool m_initiallyActive;

    void onHierarchySerialized(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(Enable);
