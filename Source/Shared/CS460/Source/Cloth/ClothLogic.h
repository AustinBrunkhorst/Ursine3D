/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ClothLogic.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Vec2.h>

struct ClothNode;

class ClothLogic
{
public:
    ClothLogic(const ursine::Vec2 &dimensions, float segmentLength);

    void Update(float dt);
    void UpdateDimensions(const ursine::Vec2 &dimensions, float segmentLength);

    ursine::uint GetWidth(void) const;
    ursine::uint GetHeight(void) const;

    ClothNode *GetNode(ursine::uint x, ursine::uint y) const;

private:

    ursine::uint m_width;
    ursine::uint m_height;

    float m_constraintLength;

    float m_stiffness;

    float m_damping;

    float m_nodeMass;

    ClothNode *m_nodes;

    std::vector<ursine::SVec3> m_positions;
    std::vector<ursine::SVec3> m_velocities;
    std::vector<ursine::SVec3> m_accelerations;

    void initializeNode(ursine::uint x, ursine::uint y) const;
    void springUpdate(ClothNode *a, ClothNode *b);
};
