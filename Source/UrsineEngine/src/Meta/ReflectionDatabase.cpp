#include "UrsinePrecompiled.h"

#include "ReflectionDatabase.h"

#include "Type.h"

namespace ursine
{
    namespace meta
    {
        ReflectionDatabase::ReflectionDatabase(void)
            : types( 1 )
            , m_nextID( 1 )
        {
            types[ Type::Invalid ].name = "UNKNOWN";
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
