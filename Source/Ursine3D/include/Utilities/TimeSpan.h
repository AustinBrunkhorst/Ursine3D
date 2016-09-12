/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TimeSpan.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineTypes.h"
#include "JsonSerializer.h"
#include <limits>

namespace ursine
{
    class TimeSpan
    {
    public:
        static const int MillisPerSecond = 1000;
        static const int MillisPerMinute = MillisPerSecond * 60;
        static const int MillisPerHour   = MillisPerMinute * 60;
        static const int MillisPerDay    = MillisPerHour * 24;

        static const int MaxMilliSeconds = INT_MAX;
        static const int MinMilliSeconds = INT_MIN;

        static const int MaxSeconds = MaxMilliSeconds / MillisPerSecond;
        static const int MinSeconds = MinMilliSeconds / MillisPerSecond;

        static const TimeSpan MaxValue;
        static const TimeSpan MinValue;

        // TimeSpan with zero milliseconds
        static const TimeSpan Zero;

        TimeSpan(int milliseconds);
        TimeSpan(float milliseconds);
        TimeSpan(int hours, int minutes, int seconds);
        TimeSpan(int days, int hours, int minutes, int seconds);
        TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);

        bool operator==(const TimeSpan &rhs) const;
        bool operator!=(const TimeSpan &rhs) const;

        bool operator<(const TimeSpan &rhs) const;
        bool operator<=(const TimeSpan &rhs) const;
        
        bool operator>(const TimeSpan &rhs) const;
        bool operator>=(const TimeSpan &rhs) const;

        TimeSpan operator-(void) const;
        TimeSpan operator-(const TimeSpan &rhs) const;
        const TimeSpan &operator-=(const TimeSpan &rhs);
        const TimeSpan &operator-=(int milliseconds);
        const TimeSpan &operator-=(float milliseconds);

        TimeSpan operator+(const TimeSpan &rhs) const;
        const TimeSpan &operator+=(const TimeSpan &rhs);
        const TimeSpan &operator+=(int milliseconds);
        const TimeSpan &operator+=(float milliseconds);

        int Milliseconds(void) const;

        int Seconds(void) const;
        int Minutes(void) const;
        int Hours(void) const;
        int Days(void) const;

        float TotalSeconds(void) const;
        float TotalMinutes(void) const;
        float TotalHours(void) const;
        float TotalDays(void) const;

        static TimeSpan FromMilliseconds(float value);
        static TimeSpan FromSeconds(float value);
        static TimeSpan FromMinutes(float value);
        static TimeSpan FromHours(float value);
        static TimeSpan FromDays(float value);

    private:
        int m_milliseconds;
    };

    extern template
    Json JsonSerializer::Serialize(TimeSpan &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, TimeSpan &out);
}