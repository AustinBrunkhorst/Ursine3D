/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IKLeafComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include "IKJoint.h"

class IKRoot;

class IKLeaf : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        std::string goalEntity,
        GetGoalEntity,
        SetGoalEntity
    );

    EditorField(
        float goalErrorMargin,
        GetGoalErrorMargin,
        SetGoalErrorMargin
    );

    IKLeaf(void);

    const std::string &GetGoalEntity(void) const;
    void SetGoalEntity(const std::string &entityName);

    float GetGoalErrorMargin(void) const;
    void SetGoalErrorMargin(float margin);

    void Initialize(IKRoot *root);

    void IKUpdate(float t);

private:

    std::string m_goalEntityName;
    ursine::ecs::EntityHandle m_goalEntity;

    float m_errorMargin;

    ursine::SVec3 m_startPosition;

    std::vector<IKJoint> m_joints;
    std::vector<float> m_distanceFromRoot;

    float getChainLength(void) const;

    void fullyExtendChain(const ursine::SVec3 &solveForPosition);
    void FABRIK(const ursine::SVec3 &solveForPosition);
    void backwardStep(const ursine::SVec3 &solveForPosition);
    void forwardStep(const ursine::SVec3 &rootPosition);

} Meta(Enable);
