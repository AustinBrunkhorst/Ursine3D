#include "UrsinePrecompiled.h"

#include "ReflectionDatabase.h"

#include "Type.h"

#define REGISTER_NATIVE_TYPE(type)                    \
    {                                                 \
        auto id = AllocateType( #type );              \
        auto &handle = types[ id ];                   \
                                                      \
        TypeInfo<type>::Register( id, handle, true ); \
    }                                                 \

namespace ursine
{
    namespace meta
    {
        ReflectionDatabase::ReflectionDatabase(void)
            : types( 1 )
            , m_nextID( 1 )
        {
            types[ Type::Invalid ].name = "UNKNOWN";

            REGISTER_NATIVE_TYPE( void );
            REGISTER_NATIVE_TYPE( int );
            REGISTER_NATIVE_TYPE( bool );
            REGISTER_NATIVE_TYPE( float );
            REGISTER_NATIVE_TYPE( double );
            REGISTER_NATIVE_TYPE( std::string );
        }

        ////////////////////////////////////////////////////////////////////////////

        ReflectionDatabase::~ReflectionDatabase(void) { }

        ////////////////////////////////////////////////////////////////////////////

        ReflectionDatabase &ReflectionDatabase::Instance(void)
        {
            static ReflectionDatabase instance;

            return instance;
        }

        ////////////////////////////////////////////////////////////////////////////

        TypeID ReflectionDatabase::AllocateType(const std::string &name)
        {
            auto search = ids.find( name );

            // already defined
            if (search != ids.end( ))
                return Type::Invalid;

            types.emplace_back( name );

            auto id = m_nextID++;

            ids[ name ] = id;

            return id;
        }

        ////////////////////////////////////////////////////////////////////////////

        const Function &ReflectionDatabase::GetGlobalFunction(const std::string &name)
        {
            auto &base = globalFunctions[ name ];

            if (!base.size( ))
                return Function::Invalid( );

            return base.begin( )->second;
        }

        ////////////////////////////////////////////////////////////////////////////

        const Function &ReflectionDatabase::GetGlobalFunction(const std::string &name, const InvokableSignature &signature)
        {
            auto &base = globalFunctions[ name ];

            auto search = base.find( signature );

            if (search == base.end( ))
                return Function::Invalid( );

            return search->second;
        }

        ////////////////////////////////////////////////////////////////////////////

        ReflectionDatabase::Initializer::Initializer(std::function<void()> initializer)
        {
            initializer( );
        }
    }
}
