/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimationRig.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <vector>

#include "SVec3.h"
#include "SQuat.h"

//this is information relating to the underlying bone structure of an animation

namespace ursine
{
    class AnimationBone
    {

    public:
        AnimationBone(void);

        /** @brief initializes a bone with proper data
        *
        *  the method will save the data, notify parent of new child,
        *  and do some checking to see if this bone is root
        *
        *  @param name name of the bone
        *  @param trans bone-space translation
        *  @param scale bone-space scale
        *  @param rotation bone-space rotation
        *  @param boneID id of this bone
        *  @param parent ptr to the parent
        *  @return Void.
        */
        void InitializeBone(
            const std::string &name, 
            const SVec3 &trans, 
            const SVec3 &scale, 
            const SQuat &rotation, 
            const unsigned boneID,
            const unsigned parentID,
            AnimationBone *parent
        );

        // is the current bone the root bone?
        bool IsRoot(void) const;

        // add a child to this bone
        void AddChild(const AnimationBone *child);

        // gettors and setters //////////////////////////////////////

        const std::string &GetName(void) const;
        void SetName(const std::string &name);

        const SVec3 &GetTranslation(void) const;
        void SetTranslation(const SVec3 &trans);

        const SVec3 &GetScale(void) const;
        void SetScale(const SVec3 &scale);

        const SQuat &GetRotation(void) const;
        void SetRotation(const SQuat &rot);

        unsigned GetBoneID(void) const;
        unsigned GetParentID(void) const;

        const AnimationBone *GetParent(void) const;

        const AnimationBone *GetChild(const unsigned childID) const;
        unsigned GetChildCount(void) const;

        const std::vector<const AnimationBone *> &GetChildren(void) const;

    private:
        std::string m_name;

        SVec3 m_translation;
        SVec3 m_scale;
        SQuat m_rotation;

        unsigned m_boneID;
        unsigned m_parentID;

        AnimationBone *m_parent;
        std::vector<const AnimationBone *> m_children;
    };
}