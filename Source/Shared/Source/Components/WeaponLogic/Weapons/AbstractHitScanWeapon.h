/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HitScanWeapon.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "AbstractWeapon.h"
#include <Raycasting.h>


#define HitscanEditorFields( )                            \
    EditorField(                                          \
        ursine::physics::RaycastType RaycastType,         \
        GetRaycastType,                                   \
        SetRaycastType                                    \
        );                                                \
                                                          \
    EditorField(                                          \
        bool DrawDuration,                                \
        GetDrawDuration,                                  \
        SetDrawDuration                                   \
    );                                                    \
    EditorField(                                          \
        bool AlwaysDraw,                                  \
        GetAlwaysDraw,                                    \
        SetAlwaysDraw                                     \
    );                                                    \
    EditorField(                                          \
        bool DebugDraw,                                   \
        GetDebug,                                         \
        SetDebug                                          \
    );


struct AbstractHitscanWeapon : AbstractWeapon
{

    AbstractHitscanWeapon(void);
    
    ursine::physics::RaycastType GetRaycastType(void) const;
    void SetRaycastType(const ursine::physics::RaycastType type);

    float GetDrawDuration(void) const;
    void SetDrawDuration(const float drawDuration);

    bool GetAlwaysDraw(void) const;
    void SetAlwaysDraw(const bool always);

    bool GetDebug(void) const;
    void SetDebug(const bool debug);

    // type of raycast (when to stop)
    ursine::physics::RaycastType m_raycastType;

    // how long to draw
    float m_drawDuration;

    // draw line
    bool m_alwaysDraw;

    // show debug info
    bool m_debug;

};
