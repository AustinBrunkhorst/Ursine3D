#include "ArrayWrapperContainer.h"

namespace ursine
{
    namespace meta
    {
        template<typename T>
        ArrayWrapper::ArrayWrapper(Array<T> &rhs)
            : m_base( new ArrayWrapperContainer<T>( rhs ) )
        {
            
        }
    }
}
