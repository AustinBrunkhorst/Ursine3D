#include "TypeInfo.h"
#include "TypeUnpacker.hpp"

#include "Variant.h"

#include "Constructor.h"

namespace ursine
{
    namespace meta
    {
        template<typename T>
        Type Type::Get(void)
        {
            return { TypeInfo< CleanedType< T > >::ID };
        }

        ////////////////////////////////////////////////////////////////////////

        template<typename T>
        Type Type::Get(T &&obj)
        {
            return { TypeInfo< CleanedType< T > >::ID };
        }

        ////////////////////////////////////////////////////////////////////////

        template<typename ... Args>
        Variant Type::Create(Args &&...args) const
        {
            static InvokableSignature signature;

            static bool initial = true;

            if (initial)
            {
                TypeUnpacker<Args...>::Apply( signature );

                initial = false;
            }

            auto &constructor = GetConstructor( signature );

            ArgumentList arguments { std::forward<Args>( args )... };

            return constructor.Invoke( arguments );
        }

        ////////////////////////////////////////////////////////////////////////

        template<typename T>
        bool Type::DerivesFrom(void) const
        {
            return DerivesFrom( Get<T>( ) );
        }
    }
}