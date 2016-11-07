/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <SVec3.h>
#include <Ease.h>

#include "EntityAnimatorClipComponent.h"

enum class EntityAnimatorEvent
{
    FinishedAnimating
};

class EntityAnimator 
    : public ursine::ecs::Component
    , public ursine::EventDispatcher<EntityAnimatorEvent>
{
    NATIVE_COMPONENT;

    friend class EntityAnimatorSystem;

public:
    // TODO: multi clips?

    BasicEditorOnlyField(bool keyPosition);
    BasicEditorOnlyField(bool keyScale);
    BasicEditorOnlyField(bool keyRotation);
    BasicEditorOnlyField(float delta);
    BasicEditorOnlyField(ursine::ease::Type ease);
    BasicEditorOnlyField(float velocity);

    EditorField(
        std::string animationClipName,
        GetAnimationClipName,
        SetAnimationClipName
    );

    EditorButton(
        saveAnimationClip,
        "Save This Animation Clip"
    );

    EditorButton(
        loadAnimationClip,
        "Load Animation By Name"
    );

    EditorButton(
        KeyValues,
        "Key Values"
    );

    EditorButton(
        play,
        "Play Animation"
    );

    EditorButton(
        pause,
        "Pause Animation"
    );

    EditorButton(
        stop,
        "Stop Animation"
    );

    EditorButton(
        drawPath,
        "Draw Path"
    );

    EditorButton(
        jumpToStart,
        "Jump To Start"
    );

    EditorButton(
        jumpToEnd,
        "Jump To End"
    );

    EditorButton(
        calculateVelocityBasedTimeValues,
        "Calculate Vel. Based Time Values"
    );

    EditorField(
        bool loopAnimation,
        GetLoopAnimation,
        SetLoopAnimation
    );

    EditorField(
        bool smoothPath,
        GetSmoothPath,
        SetSmoothPath
    );

    EditorField(
        bool playOnAwake,
        GetPlayOnAwake,
        SetPlayOnAwake
    );

    EditorField(
        bool alignToPath,
        GetAlignToPath,
        SetAlignToPath
    );

    ursine::Array<EntityKeyFrame> keyFrames;

    EntityAnimator(void);

    void OnInitialize(void) override;

    void Play(void);
    void Play(const std::string &clipName);
    void Pause(void);
    void Stop(void);

    void JumpToStart(void);
    void JumpToEnd(void);

    bool GetLoopAnimation(void) const;
    void SetLoopAnimation(bool flag);

    bool GetSmoothPath(void) const;
    void SetSmoothPath(bool flag);

    bool GetPlayOnAwake(void) const;
    void SetPlayOnAwake(bool flag);

    bool GetAlignToPath(void) const;
    void SetAlignToPath(bool flag);

    const std::string &GetAnimationClipName(void) const;
    void SetAnimationClipName(const std::string &clipName);

private:

    // Set our transform to the index passed in
    void updateAnimation(int index);

    // Linear interpolate between the two nodes passed in
    void updateAnimation(int index1, int index2, float t);

    // Smoothly interpolate between the four nodes passed in
    void updateAnimation(int index1, int index2, int index3, int index4, float t);

    // Helper for calculating values of an index 
    // (handles the case when the index doesn't have the value keyed)
    ursine::SVec3 getPosition(int index);
    ursine::SQuat getRotation(int index);
    ursine::SVec3 getScale(int index);

    // This is called when the animation completes
    void finish(void);

    bool m_playing;
    bool m_pause;
    bool m_smoothPath;
    bool m_loop;
    bool m_playOnAwake;
    bool m_alignToPath;

    // Our current index marker
    int m_index;

    // The current time marker
    float m_time;

    std::string m_clipName;

} Meta(Enable);
