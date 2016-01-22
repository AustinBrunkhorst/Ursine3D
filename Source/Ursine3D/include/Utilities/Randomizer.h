/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Randomizer.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    class Randomizer
    {
        float _min;
        float _max;

    public:
        Randomizer(void);
        Randomizer(float min, float max);

        float GetValue(void) const;

        float GetMin(void) const;
        void SetMin(float min);

        float GetMax(void) const;
        void SetMax(float max);
    };

    extern template
    Json JsonSerializer::Serialize(Randomizer &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Randomizer &out);
}
