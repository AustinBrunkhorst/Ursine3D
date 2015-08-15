/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DERPFile.h
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <vector>

namespace Ursine
{
    class DERPFile {
    private:
        class Bar {
        private:
            Bar();

            // Number of beats in every bar
            // |[][][][]| 
            // [] == beat, || == Bar
            static const int _beatsPerBar = 4;
            // Maximum intensity of a single beat
            static const int _beatMaxValue = 15;
            // One bar
            float _beats[_beatsPerBar];

            // Helpers //
            void Copy(const Bar &rhs);

        public:
            // Constructors //
            Bar(const Bar &rhs);
            Bar(int beat_0, int beat_1, int beat_2, int beat_3);

            // Operators //
            float operator[](int position) const;
            Bar &operator=(const Bar &rhs);

            // Getters //
            inline static unsigned int GetBeatsPerBar() { return _beatsPerBar; }
        };

        class Pattern {
            friend DERPFile;
        private:
            Pattern();

            // All bars within a single pattern
            std::vector<Bar> _bars;
            unsigned int _bar_itr;
            unsigned int _beat_itr;
            // Timed Pattern variables
            bool _is_timed;
            // Millisecond stop marker
            int _ms_time_limit;

            // Helpers //
            void Copy(const Pattern &rhs);

        public:
            // Constructors //
            Pattern(const Pattern &rhs);
            Pattern(std::string data, bool is_timed = false);

            // Operators //
            Pattern &operator=(const Pattern &rhs);

            // Getters //
            // returns 0f - 1f value
            float GetNextBeat();
            float GetNextBeat(bool &at_end);
            float GetNextBeat(bool &limit_reached, int timeline_position);
        };

        enum PLAY_MODE { PLAY_MODE_ORDERED, PLAY_MODE_RANDOM, PLAY_MODE_TIMED };

        // Milliseconds Per Beat
        int _mspb;
        // Helps determine when to grab the next beat
        int _last_timeline_position;
        // Store the last returned intensity
        float _last_intensity;
        // All patterns within a single DERPFile
        std::vector<Pattern> _patterns;
        unsigned int _pat_itr;
        // Playmode for this file
        PLAY_MODE _play_mode;

        // Helpers //
        static PLAY_MODE GetPlayMode(const char identifier);

    public:
        DERPFile(const std::string &file_name);
        float GetIntensity(int timeline_position);
    };
}
