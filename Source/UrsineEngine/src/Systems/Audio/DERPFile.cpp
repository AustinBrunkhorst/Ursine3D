/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DERPFile.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include <DERPFile.h>
#include "DERPFile.h"

namespace Ursine
{

    //////////////////////
    // Bar Implmentation//
    //////////////////////

    DERPFile::Bar::Bar()
    {
        memset(_beats, 0, sizeof(_beats[0]) * _beatsPerBar);
    }

    DERPFile::Bar::Bar(const DERPFile::Bar &rhs)
    {
        Copy(rhs);
    }

    // storing only 0f - 1f values
    DERPFile::Bar::Bar(int beat_0, int beat_1, int beat_2, int beat_3)
    {
        _beats[0] = Math::Clamp<int>(beat_0, 0, _beatMaxValue) / (float)_beatMaxValue;
        _beats[1] = Math::Clamp<int>(beat_1, 0, _beatMaxValue) / (float)_beatMaxValue;
        _beats[2] = Math::Clamp<int>(beat_2, 0, _beatMaxValue) / (float)_beatMaxValue;
        _beats[3] = Math::Clamp<int>(beat_3, 0, _beatMaxValue) / (float)_beatMaxValue;
    }

    // returns 0 - 1 value representing that specific beats intensity
    float DERPFile::Bar::operator[](int position) const
    {
        return _beats[position];
    }

    DERPFile::Bar &DERPFile::Bar::operator=(const DERPFile::Bar & rhs)
    {
        Copy(rhs);

        return (*this);
    }

    void DERPFile::Bar::Copy(const DERPFile::Bar &rhs)
    {
        for (int i = 0; i < _beatsPerBar; ++i)
            _beats[i] = rhs._beats[i];
    }

    ////////////////////////////
    // Pattern Implementation //
    ////////////////////////////

    DERPFile::Pattern::Pattern(const DERPFile::Pattern &rhs)
    {
        Copy(rhs);
    }

    // Expected data format(x == hex value) (00:00 == min/seconds [timed only])
    // |xxxx||xxxx||xxxx|00:00\n
    DERPFile::Pattern::Pattern(std::string data, bool is_timed)
        : _bar_itr(0)
        , _beat_itr(0)
        , _is_timed(is_timed)
        , _ms_time_limit(0)
    {
        unsigned int start_pos = 1;
        std::string str;

        // if this pattern is timed, store the stop marker
        if (is_timed)
        {
            int min, sec;

            str = data.substr(data.length() - 5, 5);
            min = Utils::StringToNumber<int>(str.substr(0, 2));
            sec = Utils::StringToNumber<int>(str.substr(3, 2));

            _ms_time_limit = (min * 60000) + (sec * 1000);

            data.erase(data.length() - 5, 5);
        }

        // while the substring doesn't contain a new line
        while (start_pos < data.length())
        {
            str = data.substr(start_pos, Bar::GetBeatsPerBar());

            int beat_0 = Utils::ASCIIHexToInt(str[0]),
                beat_1 = Utils::ASCIIHexToInt(str[1]),
                beat_2 = Utils::ASCIIHexToInt(str[2]),
                beat_3 = Utils::ASCIIHexToInt(str[3]);

            _bars.push_back(Bar(beat_0, beat_1, beat_2, beat_3));

            // increment our starting position of the substring
            // to aquire the next set of beats
            start_pos += Bar::GetBeatsPerBar() + 2;
        }
    }

    DERPFile::Pattern &DERPFile::Pattern::operator=(const DERPFile::Pattern &rhs)
    {
        Copy(rhs);

        return (*this);
    }

    void DERPFile::Pattern::Copy(const DERPFile::Pattern &rhs)
    {
        _bars = rhs._bars;
        _bar_itr = rhs._bar_itr;
        _beat_itr = rhs._beat_itr;
        _is_timed = rhs._is_timed;
        _ms_time_limit = rhs._ms_time_limit;
    }

    float DERPFile::Pattern::GetNextBeat()
    {
        // If we've arrived at the end, go back to the beginning.
        // This will result in a looping effect on one pattern.
        if (_bar_itr >= _bars.size())
        {
            _bar_itr = 0;
            _beat_itr = 0;
        }

        float value = _bars[_bar_itr][_beat_itr++];

        // Reset beat counter and incrment the bar we're on
        if (_beat_itr >= Bar::GetBeatsPerBar())
        {
            _beat_itr = 0;
            ++_bar_itr;
        }

        return value;
    }

    float DERPFile::Pattern::GetNextBeat(bool &at_end)
    {
        float value = GetNextBeat();

        at_end = (_bar_itr >= _bars.size());

        return value;
    }

    float DERPFile::Pattern::GetNextBeat(bool &limit_reached, int timeline_position)
    {
        float value = GetNextBeat();

        limit_reached = (timeline_position >= _ms_time_limit);

        return value;
    }

    /////////////////////////////
    // DERP File Implementation//
    /////////////////////////////

    DERPFile::DERPFile(const std::string &file_name) 
        : _last_timeline_position(0)
        , _pat_itr(0)
    {
        std::ifstream file(file_name);

        UAssert(file.is_open(), 
            "Error opening DERP file.\n%s", file_name.c_str());

        std::string str;

        // getting the first line (BPM)
        getline(file, str);
        _mspb = (int)(1000 / (Utils::StringToNumber<int>(str) / 60.0f));

        // Getting the second line (play mode)
        getline(file, str);
        _play_mode = GetPlayMode(str[0]);

        // Adding all patters in the file
        while (getline(file, str))
        {
            _patterns.push_back(Pattern(str, _play_mode == PLAY_MODE_TIMED));
        }

        _last_intensity = _patterns[0].GetNextBeat();
    }

    DERPFile::PLAY_MODE DERPFile::GetPlayMode(const char identifier)
    {
        static const char ORDERED = 'O';
        static const char RANDOM = 'R';
        static const char TIMED = 'T';

        switch (identifier)
        {
        case ORDERED:
            return PLAY_MODE_ORDERED;
        case RANDOM:
            return PLAY_MODE_RANDOM;
        case TIMED:
            return PLAY_MODE_TIMED;
        default:
            return PLAY_MODE_ORDERED;
        }
    }

    float DERPFile::GetIntensity(int timeline_position)
    {
        int change = timeline_position - _last_timeline_position;

        if (change >= _mspb)
        {
            bool change_pattern = false;

            switch (_play_mode)
            {
            case PLAY_MODE_ORDERED:
                _last_intensity = _patterns[_pat_itr].GetNextBeat(change_pattern);

                // If we're at the end of the pattern, incrment pattern itr
                // and check to see if it needs resetting
                if (change_pattern && ++_pat_itr >= _patterns.size())
                    _pat_itr = 0;
                break;
            case PLAY_MODE_RANDOM:
                _last_intensity = _patterns[_pat_itr].GetNextBeat(change_pattern);

                if (change_pattern)
                    _pat_itr = rand() % _patterns.size();
                break;
            case PLAY_MODE_TIMED:
                _last_intensity = _patterns[_pat_itr].GetNextBeat(change_pattern, timeline_position);

                if (change_pattern && ++_pat_itr >= _patterns.size())
                    _pat_itr = 0;
                break;
            }

            _last_timeline_position = timeline_position;
        }
        else if (change < 0)
        {
            // restart the derp file
            _pat_itr = 0;
            _last_timeline_position = 0;
            _patterns[_pat_itr]._bar_itr = 0;
            _patterns[_pat_itr]._beat_itr = 0;
            _last_intensity = _patterns[_pat_itr].GetNextBeat();
        }

        return _last_intensity;
    }
}