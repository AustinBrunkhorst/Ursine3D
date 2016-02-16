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

    CameraAnimator(void);
    ~CameraAnimator(void);

    void Play(void);
    void Reset(void);

private:

    void updateAnimation(CameraAnimatorNode *node);
    void updateAnimation(CameraAnimatorNode *node1, CameraAnimatorNode *node2, float t);

    void enableDeletionNodes(bool flag);

    void getChildren(void);
    
    // The animation nodes
    std::vector<CameraAnimatorNode*> m_nodes;
    ursine::ecs::Camera *m_camera;

    bool m_playing;
    int m_index;
    float m_time;

} Meta(Enable);
