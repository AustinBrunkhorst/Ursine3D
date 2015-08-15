#include "UrsinePrecompiled.h"

#include "Spine/spine.h"

namespace Ursine
{
    namespace Spine
    {
        IkConstraint::IkConstraint(IkConstraintData *data, const Skeleton *skeleton)
            : data(data)
            , bone_count(0)
            , bones(nullptr)
            , target(nullptr)
            , bend_direction(0)
            , mix(0)
        {
            int i;

            bend_direction = data->bend_direction;
            mix = data->mix;

            bone_count = data->bone_count;
            bones = new Bone* [bone_count];

            for (i = 0; i < bone_count; ++i)
                bones[i] = skeleton->FindBone(data->bones[i]->name.c_str());

            target = skeleton->FindBone(data->target->name.c_str());
        }

        IkConstraint::~IkConstraint(void)
        {
            delete []bones;
        }

        void IkConstraint::Apply(void)
        {
            switch (bone_count)
            {
            case 1:
                apply(bones[0], target->GetWorldX(), target->GetWorldY(), mix);
                break;
            case 2:
                apply(bones[0], bones[1], target->GetWorldX(), target->GetWorldY(), bend_direction, mix);
                break;
            }
        }

        void IkConstraint::apply(Bone *bone, float targetX, float targetY, float alpha)
        {
            float parentRotation = (!bone->GetData()->inherit_rotation || !bone->GetParent()) ? 0 : bone->GetParent()->GetWorldRotation();

            float rotation = bone->GetRotation();

            float rotationIK = Math::RadiansToDegrees(atan2(targetY - bone->GetWorldY(), targetX - bone->GetWorldX()));

            if (bone->GetWorldFlipX() != (bone->GetWorldFlipY() != (Bone::IsYDown() ? 1 : 0)))
                rotationIK = -rotationIK;

            rotationIK -= parentRotation;

            bone->SetRotationIK(rotation + (rotationIK - rotation) * alpha);
        }

        void IkConstraint::apply(Bone *parent, Bone *child, float targetX, float targetY, int bendDirection, float alpha)
        {
            float positionX;
            float positionY;
            float childX;
            float childY;
            float offset;
            float len1;
            float len2;
            float cosDenom;
            float cos;
            float childAngle;
            float adjacent;
            float opposite;
            float parentAngle;
            float rotation;

            Bone *parentParent;

            float childRotation = child->GetRotation();

            float parentRotation = parent->GetRotation();

            if (alpha == 0)
            {
                child->SetRotationIK(childRotation);
                parent->SetRotationIK(parentRotation);
                return;
            }

            parentParent = parent->GetParent();

            if (parentParent)
            {
                parentParent->WorldToLocal(targetX, targetY, &positionX, &positionY);
                targetX = (positionX - parent->GetX()) * parentParent->GetWorldScaleX();
                targetY = (positionY - parent->GetY()) * parentParent->GetWorldScaleY();
            }
            else
            {
                targetX -= parent->GetX();
                targetY -= parent->GetY();
            }
            if (child->GetParent() == parent)
            {
                positionX = child->GetX();
                positionY = child->GetY();
            }
            else
            {
                child->GetParent()->LocalToWorld(child->GetX(), child->GetY(), &positionX, &positionY);
                parent->WorldToLocal(positionX, positionY, &positionX, &positionY);
            }

            childX = positionX * parent->GetWorldScaleX();
            childY = positionY * parent->GetWorldScaleY();

            offset = atan2(childY, childX);
            len1 = sqrt(childX * childX + childY * childY);

            len2 = child->GetData()->length * child->GetWorldScaleX();

            // based on code by Ryan Juckett with permission: Copyright (c) 2008-2009 Ryan Juckett, http://www.ryanjuckett.com/
            cosDenom = 2 * len1 * len2;

            if (cosDenom < 0.0001f)
            {
                child->SetRotationIK(childRotation + (Math::RadiansToDegrees(atan2(targetY, targetX)) - parentRotation - childRotation) * alpha);
                return;
            }

            cos = (targetX * targetX + targetY * targetY - len1 * len1 - len2 * len2) / cosDenom;

            if (cos < -1)
                cos = -1;
            else if (cos > 1) /**/
                cos = 1;

            childAngle = acos(cos) * bendDirection;
            adjacent = len1 + len2 * cos;
            opposite = len2 * sin(childAngle);
            parentAngle = atan2(targetY * adjacent - targetX * opposite, targetX * adjacent + targetY * opposite);
            rotation = Math::RadiansToDegrees(parentAngle - offset) - parentRotation;

            if (rotation > 180)
                rotation -= 360;
            else if (rotation < -180) /**/
                rotation += 360;

            parent->SetRotationIK(parentRotation + rotation * alpha);
            rotation = Math::RadiansToDegrees(childAngle + offset) - childRotation;

            if (rotation > 180)
                rotation -= 360;
            else if (rotation < -180) /**/
                rotation += 360;

            child->SetRotationIK(childRotation + (rotation + parent->GetWorldRotation() - child->GetParent()->GetWorldRotation()) * alpha);
        }
    }
}
