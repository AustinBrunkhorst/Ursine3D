#pragma warning(push)

// unused template parameters
#pragma warning(disable : 4544)

namespace ursine
{
    template<typename T>
    TypeID TypeInfo<T>::ID = Type::Invalid;

    template<typename T>
    bool TypeInfo<T>::Defined = false;

    template<typename T>
    void TypeInfo<T>::Register(TypeID id, TypeData &data, bool beingDefined)
    {
        // already defined
        if (id == Type::Invalid)
            return;

        ID = id;
        
        typedef std::decay<T> Decayed;

        data.isClass = std::is_class< Decayed >::value;
        data.isEnum = std::is_enum< Decayed >::value;
        data.isPointer = std::is_pointer< T >::value;
        data.isPrimitive = std::is_arithmetic< Decayed >::value;
        
        if (beingDefined)
        {
            applyTrivialAttributes( data );
        }
    }

    template<typename T>
    template<typename U = T>
    void TypeInfo<T>::applyTrivialAttributes(TypeData &data, typename std::enable_if< !std::is_trivial<U>::value >::type* = nullptr)
    {
        // do nothing
    }

    template<typename T>
    template<typename U = T>
    void TypeInfo<T>::applyTrivialAttributes(TypeData &data, typename std::enable_if< std::is_trivial<U>::value >::type* = nullptr)
    {
        data.SetDestructor<T>( );
    }
}

#pragma warning(pop)