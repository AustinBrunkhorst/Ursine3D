/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

namespace Ursine
{
    namespace Utils
    {
        template<typename T>
        inline void Swap(T &a, T &b);

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

        template<typename T>
        std::string NumberToString(T number);

        template<typename T>
        T StringToNumber(const std::string &text);

        inline int ASCIIHexToInt(char val);

        class NoCopy
        {
        protected:
            NoCopy(void) { }
            ~NoCopy(void) { }

        private:
            NoCopy(const NoCopy &rhs) = delete;
            NoCopy &operator=(const NoCopy &rhs) = delete;
        };

        enum MediaType
        {
            TYPE_AUDIO,
            TYPE_GUIDs
        };

        std::string MediaPath(const std::string &file, MediaType type);
        std::string NormalizePath(std::string path);

        // Gets the filename from a path
        std::string FileName(const std::string &path);

        // Gets the name of the directory for a file
        std::string DirectoryName(const std::string &filename);

        // Gets the current working directory
        std::string GetWorkingDirectory(void);

        // Sets the current working directory
        void SetWorkingDirectory(const std::string &directory);
    }
}

#include "Utils.hpp"