#include "UrsinePrecompiled.h"

#include "TypeData.h"

#include "ReflectionDatabase.h"

namespace ursine
{
    namespace meta
    {
        TypeData::TypeData(void)
            : isEnum( false )
            , isPrimitive( false )
            , isPointer( false )
            , isClass( false )
            , enumeration( nullptr )
            , name( name ) { }

        ////////////////////////////////////////////////////////////////////////////

        TypeData::TypeData(const std::string &name)
            : isEnum( false )
            , isPrimitive( false )
            , isPointer( false )
            , isClass( false )
            , enumeration( nullptr )
            , name( name ) { }

        TypeData::~TypeData(void)
        {
            // free allocated enum container
            delete enumeration.m_base;

            enumeration.m_base = nullptr;
        }

        void TypeData::LoadBaseClasses(ReflectionDatabase &db, TypeID thisType, const std::initializer_list<Type> &classes)
        {
            for (auto &base : classes)
            {
                // skip invalid types
                if (base.m_id == Type::Invalid)
                    continue;

                baseClasses.insert( base );

                // add this type to the base type's derived classes
                db.types[ base.m_id ].derivedClasses.insert( thisType );
            }
        }

        ////////////////////////////////////////////////////////////////////////////

        const Constructor &TypeData::GetConstructor(const InvokableSignature &signature)
        {
            auto search = constructors.find( signature );

            if (search == constructors.end( ))
                return Constructor::Invalid( );

            return search->second;
        }

        ////////////////////////////////////////////////////////////////////////////

        const Method &TypeData::GetMethod(const std::string &name)
        {
            auto &base = methods[ name ];

            if (!base.size( ))
                return Method::Invalid( );

            return base.begin( )->second;
        }

        ////////////////////////////////////////////////////////////////////////////

        const Method &TypeData::GetMethod(const std::string &name, const InvokableSignature &signature)
        {
            auto &base = methods[ name ];

            auto search = base.find( signature );

            if (search == base.end( ))
                return Method::Invalid( );

            return search->second;
        }

        ////////////////////////////////////////////////////////////////////////////

        const Function &TypeData::GetStaticMethod(const std::string &name)
        {
            auto &base = staticMethods[ name ];

            if (!base.size( ))
                return Function::Invalid( );

            return base.begin( )->second;
        }

        ////////////////////////////////////////////////////////////////////////////

        const Function &TypeData::GetStaticMethod(const std::string &name, const InvokableSignature &signature)
        {
            auto &base = staticMethods[ name ];

            auto search = base.find( signature );

            if (search == base.end( ))
                return Function::Invalid( );

            return search->second;
        }
    }
}
