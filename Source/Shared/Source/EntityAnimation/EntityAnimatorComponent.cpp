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
#include <Color.h>
#include <DebugSystem.h>
#include <SystemManager.h>

NATIVE_COMPONENT_DEFINITION( EntityAnimator );

using namespace ursine;
using namespace ecs;

EntityAnimator::EntityAnimator(void)
    : BaseComponent( )
    , m_playing( false )
    , m_pause( false )
    , m_smoothPath( false )
    , m_loop( false )
    , m_index( 0 )
    , m_time( 0.0f )
    , m_playOnAwake( true )
{
// Initialize eitor only fields
#if defined(URSINE_WITH_EDITOR)

    keyPosition = true;
    keyScale = true;
    keyRotation = true;
    delta = 1.0f;
    ease = ease::Type::Linear;

#endif
}

void EntityAnimator::OnInitialize(void)
{
    if (m_playOnAwake)
        Play( );
}

void EntityAnimator::Play(void)
{
    m_playing = true;

    if (!m_loop)
    {
        if (m_index >= keyFrames.Size( ) - 1 || !m_pause)
            JumpToStart( );
    }

    m_pause = false;
}

void EntityAnimator::Pause(void)
{
    m_playing = false;
    m_pause = true;
}

void EntityAnimator::Stop(void)
{
    m_playing = false;

    JumpToStart( );
}

void EntityAnimator::JumpToStart(void)
{
    m_index = 0;

    updateAnimation( m_index );

    m_time = 0.0f;
}

void EntityAnimator::JumpToEnd(void)
{

    m_index = static_cast<int>( keyFrames.Size( ) ) - 1;
    
    updateAnimation( m_index );

    m_time = 0.0f;
}

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

bool EntityAnimator::GetPlayOnAwake(void) const
{
    return m_playOnAwake;
}

void EntityAnimator::SetPlayOnAwake(bool flag)
{
    m_playOnAwake = flag;
}

void EntityAnimator::updateAnimation(int index)
{
    auto trans = GetOwner( )->GetTransform( );

    // set position
    trans->SetLocalPosition( getPosition( index ) );

    // set rotation
    trans->SetLocalRotation( getRotation( index ) );

    // set scale
    trans->SetLocalScale( getScale( index ) );
}

void EntityAnimator::updateAnimation(int index1, int index2, float t)
{
    t = ease::GetFunction( keyFrames[ index2 ].ease )( t );

    auto trans = GetOwner( )->GetTransform( );

    // calculate position
    auto p1 = getPosition( index1 );
    auto p2 = getPosition( index2 );

    trans->SetLocalPosition( (1.0f - t) * p1 + t * p2 );

    // calculate rotation
    auto r1 = getRotation( index1 );
    auto r2 = getRotation( index2 );

    trans->SetLocalRotation( r1.Slerp( r2, t ) );

    // calculate the scale
    auto s1 = getScale( index1 );
    auto s2 = getScale( index2 );

    trans->SetLocalScale( (1.0f - t) * s1 + t * s2 );
}

void EntityAnimator::updateAnimation(int index1, int index2, int index3, int index4, float t)
{
    t = ease::GetFunction( keyFrames[ index3 ].ease )( t );

    auto trans = GetOwner( )->GetTransform( );

    auto p2 = getPosition( index2 );
    auto p3 = getPosition( index3 );

    if (p2 != p3)
    {
        trans->SetLocalPosition( Curves::CatmullRomSpline(
            getPosition( index1 ), p2,
            p3, getPosition( index4 ), t
        ) );
    }

    auto s2 = getScale( index2 );
    auto s3 = getScale( index3 );

    if (s2 != s3)
    {
        trans->SetLocalScale( Curves::CatmullRomSpline(
            getScale( index1 ), s2,
            s3, getScale( index4 ), t
        ) );
    }

    auto r2 = getRotation( index2 );
    auto r3 = getRotation( index3 );

    if (r2 != r3)
    {
        trans->SetLocalRotation( 
            getRotation( index2 ).Slerp( getRotation( index3 ), t ) 
        );
    }
}

SVec3 EntityAnimator::getPosition(int index)
{
    auto trans = GetOwner( )->GetTransform( );

    if (index > keyFrames.Size( ) || index < 0)
        return trans->GetLocalPosition( );

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
                return trans->GetLocalPosition( );
        }

        if (lowBound == index)
            return trans->GetLocalPosition( );

        if (keyFrames[ lowBound ].positionKey)
            break;

        --lowBound;

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

        ++highBound;

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
        return trans->GetLocalRotation( );

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
                return trans->GetLocalRotation( );
        }

        if (lowBound == index)
            return trans->GetLocalRotation( );

        if (keyFrames[ lowBound ].rotationKey)
            break;

        --lowBound;

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

        ++highBound;

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

        --lowBound;

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

        ++highBound;

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
        key.position = trans->GetLocalPosition( );
        key.positionKey = true;
    }
    if (keyScale)
    {
        key.scale = trans->GetLocalScale( );
        key.scaleKey = true;
    }
    if (keyRotation)
    {
        key.rotation = trans->GetLocalRotation( );
        key.rotationKey = true;
    }

    key.ease = ease;
    key.delta = delta;

    keyFrames.Push( 
        key
    );
}

void EntityAnimator::play(void)
{
    Play( );
}

void EntityAnimator::pause(void)
{
    Pause( );
}

void EntityAnimator::stop(void)
{
    Stop( );
}

void EntityAnimator::drawPath(void)
{
    auto drawer = GetOwner( )->GetWorld( )->GetEntitySystem<DebugSystem>( );

    if (m_smoothPath)
    {
        for (int i = 0; i < keyFrames.Size( ); ++i)
        {
            auto max = static_cast<int>( keyFrames.Size( ) ) - 1;

            if (!m_loop && i >= max)
                continue;

            auto node0 = getPosition( i );
            auto node1 = getPosition( i == max ? 0 : i + 1 );
            
            auto before = getPosition( i == 0 ? max : i - 1 );
            auto after = getPosition( i == max ? 1 : i + 2 );

            static const int precision = 20;
            float t = 0.0f;
            float step = 1.0f / precision;

            for (int j = 0; j < precision; ++j)
            {
                auto p0 = Curves::CatmullRomSpline( before, node0, node1, after, t );

                t += step;

                auto p1 = Curves::CatmullRomSpline( before, node0, node1, after, t );

                drawer->DrawLine( p0, p1, Color::Yellow, 10.0f );
            }
        }
    }
    else
    {
        for (int i = 0; i < keyFrames.Size( ); ++i)
        {
            auto max = keyFrames.Size( ) - 1;

            if (!m_loop && i >= max)
                continue;

            auto node0 = getPosition( i );
            auto node1 = getPosition( i == max ? 0 : i + 1 );

            drawer->DrawLine( node0, node1, Color::Yellow, 10.0f );
        }
    }
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
