/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Utils.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "UrsineMath.h"
#include "MacroUtils.h"

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

namespace ursine
{
    namespace utils
    {
        template<typename A, typename B>
        inline bool IsFlagSet(A value, B flag);

        template<typename A, typename B>
        inline void FlagSet(A &value, B flag);

        template<typename A, typename B>
        inline void FlagUnset(A &value, B flag);

        template<typename A>
        inline A FlagNegateAll(A value);

        // Insertions sorts the given container
        template<typename Container, class Predicate>
        void InsertionSort(Container &v, Predicate predicate);

        // Sort inserts into a given container (assuming it's already sorted)
        template<typename Container, class T, class Predicate>
        void InsertionSort(Container &v, const T &item, Predicate predicate);

        void Join(
            const std::vector<std::string> &input,
            const std::string &joiner,
            std::string &output);

        class NoCopy
        {
        protected:
            NoCopy(void) { }
            ~NoCopy(void) { }

        private:
            NoCopy(const NoCopy &rhs) = delete;
            NoCopy &operator=(const NoCopy &rhs) = delete;
        };
    }
}

#include "Utils.hpp"