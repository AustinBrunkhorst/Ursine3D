/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EntityAnimatorComponent.h"

#include <Curves.h>

NATIVE_COMPONENT_DEFINITION( EntityAnimator );

using namespace ursine;
using namespace ecs;

EntityAnimator::EntityAnimator(void)
    : BaseComponent( )
    , m_playing( false )
    , m_smoothPath( false )
    , m_loop( false )
    , m_index( 0 )
    , m_time( 0.0f )
    //, m_easingFunction( ease::Linear )
{
}

void EntityAnimator::Play(void)
{
    m_playing = true;
}

void EntityAnimator::JumpToStart(void)
{
    updateAnimation( 0 );
}

void EntityAnimator::JumpToEnd(void)
{
    updateAnimation( static_cast<int>( keyFrames.Size( ) ) - 1 );
}

/*ease::Function EntityAnimator::GetEasingFunction(void) const
{
    return m_easingFunction;
}

void EntityAnimator::SetEasingFunction(ease::Function func)
{
    m_easingFunction = func;
}*/

bool EntityAnimator::GetLoopAnimation(void) const
{
    return m_loop;
}

void EntityAnimator::SetLoopAnimation(bool flag)
{
    m_loop = flag;
}

bool EntityAnimator::GetSmoothPath(void) const
{
    return m_smoothPath;
}

void EntityAnimator::SetSmoothPath(bool flag)
{
    m_smoothPath = flag;
}

void EntityAnimator::updateAnimation(int index)
{
    auto trans = GetOwner( )->GetTransform( );

    // set position
    trans->SetWorldPosition( getPosition( index ) );

    // set rotation
    trans->SetWorldRotation( getRotation( index ) );

    // set scale
    trans->SetLocalScale( getScale( index ) );
}

void EntityAnimator::updateAnimation(int index1, int index2, float t)
{
    auto trans = GetOwner( )->GetTransform( );

    // calculate position
    auto p1 = getPosition( index1 );
    auto p2 = getPosition( index2 );

    trans->SetWorldPosition( (1.0f - t) * p1 + t * p2 );

    // calculate rotation
    auto r1 = getRotation( index1 );
    auto r2 = getRotation( index2 );

    trans->SetWorldRotation( r1.Slerp( r2, t ) );

    // calculate the scale
    auto s1 = getScale( index1 );
    auto s2 = getScale( index2 );

    trans->SetLocalScale( (1.0f - t) * s1 + t * s2 );
}

void EntityAnimator::updateAnimation(int index1, int index2, int index3, int index4, float t)
{
    auto trans = GetOwner( )->GetTransform( );

    trans->SetWorldPosition( Curves::CatmullRomSpline(
        getPosition( index1 ), getPosition( index2 ),
        getPosition( index3 ), getPosition( index4 ), t
    ) );

    trans->SetLocalScale( Curves::CatmullRomSpline(
        getScale( index1 ), getScale( index2 ),
        getScale( index3 ), getScale( index4 ), t
    ) );

    trans->SetWorldRotation( 
        getRotation( index2 ).Slerp( getRotation( index3 ), t ) 
    );
}

SVec3 EntityAnimator::getPosition(int index)
{
    auto trans = GetOwner( )->GetTransform( );

    if (index > keyFrames.Size( ) || index < 0)
        return trans->GetWorldPosition( );

    if (keyFrames[ index ].positionKey)
        return keyFrames[ index ].position;

    // search for the closest index that is keyed in both directions
    int lowBound = index - 1;

    do
    {
        if (lowBound < 0)
        {
            if (m_loop)
                lowBound = static_cast<int>( keyFrames.Size( ) ) - 1;
            else
                return trans->GetWorldPosition( );
        }

        if (lowBound == index)
            return trans->GetWorldPosition( );

        if (keyFrames[ lowBound ].positionKey)
            break;

    } while (true);

    int highBound = index + 1;

    do
    {
        if (highBound >= keyFrames.Size( ))
        {
            if (m_loop)
                highBound = 0;
            else
                return keyFrames[ lowBound ].position;
        }

        if (highBound == lowBound)
            return keyFrames[ lowBound ].position;

        if (keyFrames[ highBound ].positionKey)
            break;

    } while (true);

    // get the positions of the high and low
    auto &highPos = keyFrames[ highBound ].position;
    auto &lowPos = keyFrames[ lowBound ].position;

    // get the fraction for the lerp
    if (highBound < index)
        highBound = static_cast<int>( keyFrames.Size( ) ) + highBound;
    if (lowBound > index)
        lowBound = -lowBound;

    int dist = abs( highBound - lowBound ) + 1;

    float t = (index - lowBound) / static_cast<float>( dist );

    return (1.0f - t) * lowPos + t * highPos;
}

SQuat EntityAnimator::getRotation(int index)
{
    auto trans = GetOwner( )->GetTransform( );

    if (index > keyFrames.Size( ) || index < 0)
        return trans->GetWorldRotation( );

    if (keyFrames[ index ].rotationKey)
        return keyFrames[ index ].rotation;

    // search for the closest index that is keyed in both directions
    int lowBound = index - 1;

    do
    {
        if (lowBound < 0)
        {
            if (m_loop)
                lowBound = static_cast<int>( keyFrames.Size( ) ) - 1;
            else
                return trans->GetWorldRotation( );
        }

        if (lowBound == index)
            return trans->GetWorldRotation( );

        if (keyFrames[ lowBound ].rotationKey)
            break;

    } while (true);

    int highBound = index + 1;

    do
    {
        if (highBound >= keyFrames.Size( ))
        {
            if (m_loop)
                highBound = 0;
            else
                return keyFrames[ lowBound ].rotation;
        }

        if (highBound == lowBound)
            return keyFrames[ lowBound ].rotation;

        if (keyFrames[ highBound ].rotationKey)
            break;

    } while (true);

    // get the positions of the high and low
    auto &highRot = keyFrames[ highBound ].rotation;
    auto &lowRot = keyFrames[ lowBound ].rotation;

    // get the fraction for the lerp
    if (highBound < index)
        highBound = static_cast<int>( keyFrames.Size( ) ) + highBound;
    if (lowBound > index)
        lowBound = -lowBound;

    int dist = abs( highBound - lowBound ) + 1;

    float t = (index - lowBound) / static_cast<float>( dist );

    return lowRot.Slerp( highRot, t );
}

SVec3 EntityAnimator::getScale(int index)
{
    auto trans = GetOwner( )->GetTransform( );

    if (index > keyFrames.Size( ) || index < 0)
        return trans->GetLocalScale( );

    if (keyFrames[ index ].scaleKey)
        return keyFrames[ index ].scale;

    // search for the closest index that is keyed in both directions
    int lowBound = index - 1;

    do
    {
        if (lowBound < 0)
        {
            if (m_loop)
                lowBound = static_cast<int>( keyFrames.Size( ) ) - 1;
            else
                return trans->GetLocalScale( );
        }

        if (lowBound == index)
            return trans->GetLocalScale( );

        if (keyFrames[ lowBound ].scaleKey)
            break;

    } while (true);

    int highBound = index + 1;

    do
    {
        if (highBound >= keyFrames.Size( ))
        {
            if (m_loop)
                highBound = 0;
            else
                return keyFrames[ lowBound ].scale;
        }

        if (highBound == lowBound)
            return keyFrames[ lowBound ].scale;

        if (keyFrames[ highBound ].scaleKey)
            break;

    } while (true);

    // get the positions of the high and low
    auto &highScale = keyFrames[ highBound ].scale;
    auto &lowScale = keyFrames[ lowBound ].scale;

    // get the fraction for the lerp
    if (highBound < index)
        highBound = static_cast<int>( keyFrames.Size( ) ) + highBound;
    if (lowBound > index)
        lowBound = -lowBound;

    int dist = abs( highBound - lowBound ) + 1;

    float t = (index - lowBound) / static_cast<float>( dist );

    return (1.0f - t) * lowScale + t * highScale;
}

void EntityAnimator::finish(void)
{
    m_playing = false;

    Dispatch( EntityAnimatorEvent::FinishedAnimating, EventArgs::Empty );
}

#if defined(URSINE_WITH_EDITOR)

void EntityAnimator::KeyValues(void)
{
    EntityKeyFrame key;
    auto trans = GetOwner( )->GetTransform( );

    if (!keyPosition && !keyScale && !keyRotation)
        return;

    if (keyPosition)
    {
        key.position = trans->GetWorldPosition( );
        key.positionKey = true;
    }
    if (keyScale)
    {
        key.scale = trans->GetLocalScale( );
        key.scaleKey = true;
    }
    if (keyRotation)
    {
        key.rotation = trans->GetWorldRotation( );
        key.rotationKey = true;
    }

    keyFrames.Push( 
        key
    );
}

void EntityAnimator::play(void)
{
    Play( );
}

void EntityAnimator::jumpToStart(void)
{
    JumpToStart( );
}

void EntityAnimator::jumpToEnd(void)
{
    JumpToEnd( );
}

#endif
