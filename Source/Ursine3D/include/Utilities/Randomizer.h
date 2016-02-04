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

#include <random>

namespace ursine
{
    class Randomizer
    {
    public:
        Randomizer(void);
        Randomizer(float min, float max);

        float GetValue(void);

        float GetMin(void) const;
        void SetMin(float min);

        float GetMax(void) const;
        void SetMax(float max);

    private:
        std::uniform_real_distribution<float> m_distributor;
        std::default_random_engine m_engine;
    };

    extern template
    Json JsonSerializer::Serialize(Randomizer &instance);

    extern template
    void JsonSerializer::Deserialize(const Json &data, Randomizer &out);
}
