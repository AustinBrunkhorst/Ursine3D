/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IKJoint.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

class IKJoint
{
public:
    ursine::SQuat worldBindPose;
    ursine::SVec3 childWorldDirection;
    ursine::ecs::Transform *transform;

    IKJoint(void);
    IKJoint(ursine::ecs::Transform *transform);

    ursine::SVec3 GetWorldPosition(void) const;
    void SetWorldPosition(const ursine::SVec3 &position);

    void UpdateOrientation(const ursine::SVec3 &goalPosition);
    void CorrectOrientation(const ursine::SVec3 &goalPosition) const;
};
