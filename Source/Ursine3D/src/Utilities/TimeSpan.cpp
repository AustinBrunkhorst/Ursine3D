/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimeSpan.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TimeSpan.h"

namespace ursine
{
    const TimeSpan TimeSpan::MaxValue( MaxMilliSeconds );
    const TimeSpan TimeSpan::MinValue( MinMilliSeconds );

    const TimeSpan TimeSpan::Zero( 0 );

    TimeSpan::TimeSpan(int milliseconds)
        : m_milliseconds( milliseconds )
    {
    }

    TimeSpan::TimeSpan(float milliseconds)
        : m_milliseconds( static_cast<int>( milliseconds ) )
    {
    }

    TimeSpan::TimeSpan(int hours, int minutes, int seconds)
        : TimeSpan( (hours * 3600 + minutes * 60 + seconds) * MillisPerSecond )
    {
    }

    TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
        : TimeSpan( days, hours, minutes, seconds, 0 )
    {
    }

    TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
        : TimeSpan( days * 3600 * 24 + hours * 3600 + minutes * 60 + seconds * 1000 + milliseconds )
    {
    }

    bool TimeSpan::operator==(const TimeSpan &rhs) const
    {
        return m_milliseconds == rhs.m_milliseconds;
    }

    bool TimeSpan::operator!=(const TimeSpan &rhs) const
    {
        return m_milliseconds != rhs.m_milliseconds;
    }

    bool TimeSpan::operator<(const TimeSpan &rhs) const
    {
        return m_milliseconds < rhs.m_milliseconds;
    }

    bool TimeSpan::operator<=(const TimeSpan &rhs) const
    {
        return m_milliseconds <= rhs.m_milliseconds;
    }

    bool TimeSpan::operator>(const TimeSpan &rhs) const
    {
        return m_milliseconds > rhs.m_milliseconds;
    }

    bool TimeSpan::operator>=(const TimeSpan &rhs) const
    {
        return m_milliseconds >= rhs.m_milliseconds;
    }

    TimeSpan TimeSpan::operator-(void) const
    {
        return TimeSpan( -m_milliseconds );
    }

    TimeSpan TimeSpan::operator-(const TimeSpan &rhs) const
    {
        return TimeSpan( m_milliseconds - rhs.m_milliseconds );
    }

    const TimeSpan &TimeSpan::operator-=(const TimeSpan &rhs)
    {
        m_milliseconds -= rhs.m_milliseconds;

        return *this;
    }

    const TimeSpan &TimeSpan::operator-=(int milliseconds)
    {
        m_milliseconds -= milliseconds;

        return *this;
    }

    const TimeSpan &TimeSpan::operator-=(float milliseconds)
    {
        m_milliseconds -= static_cast<int>( milliseconds );

        return *this;
    }

    TimeSpan TimeSpan::operator+(const TimeSpan &rhs) const
    {
        return TimeSpan( m_milliseconds + rhs.m_milliseconds );
    }

    const TimeSpan &TimeSpan::operator+=(const TimeSpan &rhs)
    {
        m_milliseconds += rhs.m_milliseconds;

        return *this;
    }

    const TimeSpan &TimeSpan::operator+=(int milliseconds)
    {
        m_milliseconds += milliseconds;

        return *this;
    }

    const TimeSpan &TimeSpan::operator+=(float milliseconds)
    {
        m_milliseconds += static_cast<int>( milliseconds );

        return *this;
    }

    int TimeSpan::Milliseconds(void) const
    {
        return m_milliseconds;
    }

    int TimeSpan::Seconds(void) const
    {
        return static_cast<int>( fmod( m_milliseconds / static_cast<float>( MillisPerSecond ), 60.0f ) );
    }

    int TimeSpan::Minutes(void) const
    {
        return static_cast<int>( fmod( m_milliseconds / static_cast<float>( MillisPerMinute ), 60.0f ) );
    }

    int TimeSpan::Hours(void) const
    {
        return static_cast<int>( fmod( m_milliseconds / static_cast<float>( MillisPerHour ), 24.0f ) );
    }

    int TimeSpan::Days(void) const
    {
        return static_cast<int>( m_milliseconds / MillisPerDay );
    }

    float TimeSpan::TotalSeconds(void) const
    {
        return m_milliseconds / static_cast<float>( MillisPerSecond );
    }

    float TimeSpan::TotalMinutes(void) const
    {
        return m_milliseconds / static_cast<float>( MillisPerMinute );
    }

    float TimeSpan::TotalHours(void) const
    {
        return m_milliseconds / static_cast<float>( MillisPerHour );
    }

    float TimeSpan::TotalDays(void) const
    {
        return m_milliseconds / static_cast<float>( MillisPerDay );
    }

    TimeSpan TimeSpan::FromMilliseconds(float value)
    {
        return TimeSpan( static_cast<int>( value ) );
    }

    TimeSpan TimeSpan::FromSeconds(float value)
    {
        return TimeSpan( static_cast<int>( value * MillisPerSecond ) );
    }

    TimeSpan TimeSpan::FromMinutes(float value)
    {
        return TimeSpan( static_cast<int>( value * MillisPerMinute ) );
    }

    TimeSpan TimeSpan::FromHours(float value)
    {
        return TimeSpan( static_cast<int>( value * MillisPerHour ) );
    }

    TimeSpan TimeSpan::FromDays(float value)
    {
        return TimeSpan( static_cast<int>( value * MillisPerDay ) );
    }

    template<>
    Json JsonSerializer::Serialize(TimeSpan &instance)
    {
        return Json( instance.Milliseconds( ) );
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, TimeSpan &out)
    {
        UAssert(data.is_number(), "Timespan is serialized in milliseconds in the form of an integer.");
        out = TimeSpan( data.int_value( ) );
    }
}
