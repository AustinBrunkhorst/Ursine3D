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

struct EntityKeyFrame
{
    EntityKeyFrame(void)
        : delta( 0.0f )
        , positionKey( false )
        , scaleKey( false )
        , rotationKey( false ) { }

    bool positionKey;
    bool scaleKey;
    bool rotationKey;

    ursine::SVec3 position;
    ursine::SVec3 scale;
    ursine::SQuat rotation;

    float delta;

} Meta(Enable, EnableArrayType);

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
    // TODO: easing, draw path, broke
    
    EditorOnlyField(bool keyPosition);
    EditorOnlyField(bool keyScale);
    EditorOnlyField(bool keyRotation);

    EditorButton(
        KeyValues,
        "Key Values"
    );

    EditorButton(
        play,
        "Play Animation"
    );

    EditorButton(
        jumpToStart,
        "Jump To Start"
    );

    EditorButton(
        jumpToEnd,
        "Jump To End"
    );

    /*EditorField(
        ursine::ease::Function easingFunction,
        GetEasingFunction,
        SetEasingFunction
    );*/

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

    EntityAnimator(void);

    void Play(void);

    void JumpToStart(void);
    void JumpToEnd(void);

    /*ursine::ease::Function GetEasingFunction(void) const;
    void SetEasingFunction(ursine::ease::Function func);*/

    bool GetLoopAnimation(void) const;
    void SetLoopAnimation(bool flag);

    bool GetSmoothPath(void) const;
    void SetSmoothPath(bool flag);

    ursine::Array<EntityKeyFrame> keyFrames;

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
    bool m_smoothPath;
    bool m_loop;

    // Our current index marker
    int m_index;

    // The current time marker
    float m_time;

    //ursine::ease::Function m_easingFunction;

} Meta(Enable);
