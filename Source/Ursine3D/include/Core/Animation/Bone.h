/* ---------------------------------------------------------------------------
** Team Bear King
** @ 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Bone.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class Bone
    {
    public:
        Bone(void);

        // Local position, scale, rotation
        SVec3 localPosition;
        SVec3 localScale;
        SQuat localRotation;

        SMat4 offset;

        uint parent;
        uint numChildren;

        std::string name;
    };
}
