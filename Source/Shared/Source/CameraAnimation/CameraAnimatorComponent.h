/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAnimatorComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class CameraAnimatorNode;

namespace ursine
{
    namespace ecs
    {
        class Camera;
    }
}

class CameraAnimator : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

    friend class CameraAnimatorSystem;

public:
    EditorButton(
        play,
        "Play Animation"        
    );

    EditorButton(
        reset,
        "Reset"
    );

    EditorButton(
        drawPath,
        "Draw Path"
    );

    EditorField(
        bool smoothPath,
        GetSmoothPath,
        SetSmoothPath
    );

    CameraAnimator(void);
    ~CameraAnimator(void);

    bool GetSmoothPath(void) const;
    void SetSmoothPath(bool smooth);

    void Play(void);
    void Reset(void);

private:

    // Set our position and rotation equal to the node passed in
    void updateAnimation(CameraAnimatorNode *node);

    // Linear interpolate between the two nodes passed in
    void updateAnimation(CameraAnimatorNode *node1, CameraAnimatorNode *node2, float t);

    // Smoothly interpolate between the four nodes passed in
    void updateAnimation(CameraAnimatorNode *node1, CameraAnimatorNode *node2,
                         CameraAnimatorNode *node3, CameraAnimatorNode *node4, float t);

    void enableDeletionNodes(bool flag);

    void getChildren(void);
    
    bool m_playing    : 1;
    bool m_smoothPath : 1;

    // The camera we're animating
    ursine::ecs::Camera *m_camera;

    // The animation nodes
    std::vector<CameraAnimatorNode*> m_nodes;
    
    // Our current index marker
    int m_index;

    // The current time marker
    float m_time;

} Meta(Enable);
