/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IKRootComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class IKLeaf;

class IKRoot : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        float t,
        GetT,
        SetT
    );

    IKRoot(void);

    void OnInitialize(void) override;

    float GetT(void) const;
    void SetT(float t);

private:

    float m_t;
    std::vector<IKLeaf*> m_leaves;

    void onAnimationManipulation(EVENT_HANDLER(ursine::ecs::Entity));
    void onHierarchySerialized(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(Enable);
