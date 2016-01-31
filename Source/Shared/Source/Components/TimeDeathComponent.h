/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** TimeDeathComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>


class TimeDeath : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        float TimeBeforeDeath,
        GetTimeBeforeDeath,
        SetTimeBeforeDeath
        );

    TimeDeath( void );

    float GetTimeBeforeDeath( void );
    void SetTimeBeforeDeath( const float time );

    void DecrementTimer( const float time );

private:

    // Time allotted before killing owner
    float m_timeBeforeDeath;

} Meta( Enable, DisplayName( "TimeDeath" ) );
