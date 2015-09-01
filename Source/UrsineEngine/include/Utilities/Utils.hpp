/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Utils.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

namespace ursine
{
    namespace utils
    {
        template<typename T>
        void Swap(T &a, T &b)
        {
            T temp = a;
            a = b;
            b = temp;
        }

        template<typename A, typename B>
        inline bool IsFlagSet(A value, B flag)
        {
            return (value & flag) == flag;
        }

        template<typename A, typename B>
        inline void FlagSet(A &value, B flag)
        {
            value |= flag;
        }

        template<typename A, typename B>
        inline void FlagUnset(A &value, B flag)
        {
            value &= ~flag;
        }

        template<typename A>
        inline A FlagNegateAll(A value)
        {
            return ~value;
        }

        template<typename Container, class Predicate>
        void InsertionSort(Container &v, Predicate predicate)
        {
            for (auto i = v.begin(); i != v.end(); ++i) 
            {
                std::rotate(std::upper_bound(v.begin(), i, *i, predicate), i, i + 1);
            }
        }

        template<typename Container, class T, class Predicate>
        void InsertionSort(Container &v, const T &item, Predicate predicate)
        {
            if (v.empty())
            {
                v.push_back(item);
            }
            else
            {
                v.insert(std::upper_bound(v.begin(), v.end(), item, predicate), item);
            }
        }

        template<typename T>
        std::string NumberToString(T number)
        {
            std::stringstream ss;
            ss << number;
            return ss.str();
        }

        template<typename T>
        T StringToNumber(const std::string &text)
        {
            std::stringstream ss(text);
            T result;
            return ss >> result ? result : 0;
        }

        inline int ASCIIHexToInt(char val)
        {
            if (val <= '9')
                return math::Clamp<int>(val - '0', 0, 9);
                
            return math::Clamp<int>((val - 'A') + 10, 10, 16);
        }
    }
}