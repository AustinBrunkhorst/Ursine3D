#pragma once

#include "VariantBase.h"

#include "TypeConfig.h"

#include <vector>
#include <memory>

namespace ursine
{
    namespace meta
    {
        class Variant
        {
        public:
            Variant(void);

            template<typename T>
            Variant(T *data
                , typename std::enable_if< std::is_base_of<Object, T>::value >::type* = nullptr
            );

            template<typename T>
            Variant(T &data);

            template<typename T>
            Variant(T &&data
                , typename std::enable_if< !std::is_same<Variant&, T>::value >::type* = nullptr
                , typename std::enable_if< !std::is_const<T>::value >::type* = nullptr
            );

            Variant(const Variant &rhs);
            Variant(Variant &&rhs);

            ~Variant(void);

            template<typename T>
            Variant &operator=(T &&rhs);

            Variant &operator=(Variant &&rhs);
            Variant &operator=(const Variant &rhs);

            operator bool(void) const;

            Type GetType(void) const;

            void Swap(Variant &other);

            int ToInt(void) const;
            bool ToBool(void) const;
            float ToFloat(void) const;
            double ToDouble(void) const;
            std::string ToString(void) const;

            template<typename T>
            T &GetValue(void) const;

            bool IsValid(void) const;
            bool IsConst(void) const;

        private:
            friend class Argument;
            friend class Destructor;

            void *getPtr(void) const;

            bool m_isConst;

            VariantBase *m_base;
        };
    }
}

#include "Impl/Variant.hpp"