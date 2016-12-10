/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ClothComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "ClothLogic.h"

class Cloth : public ursine::ecs::Component
{
  NATIVE_COMPONENT;

public:

    EditorField(
      ursine::Vec2 dimensions,
      GetDimensions,
      SetDimensions
    );

    EditorMeta(InputRange( 1.0f, 100.0f ))
    EditorField(
      float segmentLength,
      GetSegmentLength,
      SetSegmentLength
    );

    EditorOnlyField(
        bool debugDraw,
        GetDebugDraw,
        SetDebugDraw
    );

    Cloth(void);

    const ursine::Vec2 &GetDimensions(void) const;
    void SetDimensions(const ursine::Vec2 &dimensions);

    float GetSegmentLength(void) const;
    void SetSegmentLength(float segmentLength);

#if defined(URSINE_WITH_EDITOR)

    bool GetDebugDraw(void) const;
    void SetDebugDraw(bool active);

    void DebugDraw(void);

#endif

private:

    ursine::Vec2 m_dimensions;
    float m_segmentLength;

    ClothLogic m_logic;

    bool m_debug;

} Meta(Enable);
