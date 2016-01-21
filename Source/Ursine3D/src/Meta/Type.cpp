/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Type.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Type.h"

#include "Variant.h"
#include "Enum.h"

#include "Constructor.h"
#include "Destructor.h"
#include "Field.h"
#include "Method.h"
#include "Function.h"

#include "MetaManager.h"

#include "ReflectionDatabase.h"

namespace ursine
{
    namespace meta
    {
        namespace
        {
            // static database instance
            auto &database = ReflectionDatabase::Instance( );
        }

        Type::Type(void)
            : m_id( Invalid )
            , m_isArray( false ) { }

        ///////////////////////////////////////////////////////////////////////

        Type::Type(const Type &rhs)
            : m_id( rhs.m_id )
            , m_isArray( rhs.m_isArray ) { }

        ///////////////////////////////////////////////////////////////////////

        Type::Type(TypeID id, bool isArray)
            : m_id( id )
            , m_isArray( isArray ) { }

        ///////////////////////////////////////////////////////////////////////

        Type::operator bool(void) const
        {
            return m_id != Invalid;
        }

        ///////////////////////////////////////////////////////////////////////

        Type &Type::operator=(const Type &rhs)
        {
            m_id = rhs.m_id;
            m_isArray = rhs.m_isArray;

            return *this;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator<(const Type &rhs) const
        {
            return m_id < rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator>(const Type &rhs) const
        {
            return m_id > rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator<=(const Type &rhs) const
        {
            return m_id <= rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator>=(const Type &rhs) const
        {
            return m_id >= rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator==(const Type &rhs) const
        {
            return m_id == rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::operator!=(const Type &rhs) const
        {
            return m_id != rhs.m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        TypeID Type::GetID(void) const
        {
            return m_id;
        }

        ///////////////////////////////////////////////////////////////////////

        Type::List Type::GetTypes(void)
        {
            auto count = database.types.size( );

            List types;

            // skip the first one, as it's reserved for unknown
            for (TypeID i = 1; i < count; ++i)
                types.emplace_back( i );

            return types;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Global> Type::GetGlobals(void)
        {
            std::vector<Global> globals;

            for (auto &global : database.globals)
                globals.emplace_back( global.second );

            return globals;
        }

        ///////////////////////////////////////////////////////////////////////

        const Global &Type::GetGlobal(const std::string &name)
        {
            return database.globals[ name ];
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Function> Type::GetGlobalFunctions(void)
        {
            std::vector<Function> functions;

            for (auto &overload : database.globalFunctions)
            {
                for (auto &function : overload.second)
                {
                    functions.emplace_back( function.second );
                }
            }

            return functions;
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetGlobalFunction(const std::string &name)
        {
            return database.GetGlobalFunction( name );
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetGlobalFunction(
            const std::string &name, 
            const InvokableSignature &signature
        )
        {
            return database.GetGlobalFunction( name, signature );
        }

        ///////////////////////////////////////////////////////////////////////

        Type Type::GetFromName(const std::string &name)
        {
            auto search = database.ids.find( name );

            if (search == database.ids.end( ))
                return { Invalid };

            return { search->second };
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::ListsEqual(const List &a, const List &b)
        {
            if (a.size( ) != b.size( ))
                return false;

            auto equal = true;

            for (size_t i = 0u, size = a.size( ); equal && i < size; ++i)
                equal = (a[ i ] == b[ i ]);

            return equal;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsValid(void) const
        {
            return m_id != Invalid;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsPrimitive(void) const
        {
            return database.types[ m_id ].isPrimitive;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsFloatingPoint(void) const
        {
            return database.types[ m_id ].isFloatingPoint;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsSigned(void) const
        {
            return database.types[ m_id ].isSigned;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsEnum(void) const
        {
            return database.types[ m_id ].isEnum;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsPointer(void) const
        {
            return database.types[ m_id ].isPointer;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsClass(void) const
        {
            return database.types[ m_id ].isClass;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::IsArray(void) const
        {
            return m_isArray;
        }

        ///////////////////////////////////////////////////////////////////////

        const std::string &Type::GetName(void) const
        {
            return database.types[ m_id ].name;
        }

        const MetaManager &Type::GetMeta(void) const
        {
            return database.types[ m_id ].meta;
        }

        ///////////////////////////////////////////////////////////////////////

        Variant Type::CreateVariadic(const ArgumentList &arguments) const
        {
            InvokableSignature signature;

            for (auto &argument : arguments)
                signature.emplace_back( argument.GetType( ) );

            auto &constructor = GetConstructor( signature );

            return constructor.Invoke( arguments );
        }

        Variant Type::CreateDynamicVariadic(const ArgumentList &arguments) const
        {
            InvokableSignature signature;

            for (auto &argument : arguments)
                signature.emplace_back( argument.GetType( ) );

            auto &constructor = GetDynamicConstructor( signature );

            return constructor.Invoke( arguments );
        }

        ///////////////////////////////////////////////////////////////////////

        void Type::Destroy(Variant &instance) const
        {
            auto &destructor = database.types[ m_id ].destructor;

            if (destructor.IsValid( ))
                destructor.Invoke( instance );
        }

        ///////////////////////////////////////////////////////////////////////

        Type Type::GetDecayedType(void) const
        {
            URSINE_TODO( "convert to non pointer/const pointer type" );
            return Type( );
        }

        ///////////////////////////////////////////////////////////////////////

        Type Type::GetArrayType(void) const
        {
            return Type( m_id, false );
        }

        ///////////////////////////////////////////////////////////////////////

        const Enum &Type::GetEnum(void) const
        {
            return database.types[ m_id ].enumeration;
        }

        ///////////////////////////////////////////////////////////////////////

        bool Type::DerivesFrom(const Type &other) const
        {
            auto &baseClasses = database.types[ m_id ].baseClasses;

            return baseClasses.find( other ) != baseClasses.end( );
        }

        ///////////////////////////////////////////////////////////////////////

        const Type::Set &Type::GetBaseClasses(void) const
        {
            return database.types[ m_id ].baseClasses;
        }

        ///////////////////////////////////////////////////////////////////////

        const Type::Set &Type::GetDerivedClasses(void) const
        {
            return database.types[ m_id ].derivedClasses;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Constructor> Type::GetConstructors(void) const
        {
            auto &handle = database.types[ m_id ].constructors;

            std::vector<Constructor> constructors;

            for (auto &constructor : handle)
                constructors.emplace_back( constructor.second );

            return constructors;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Constructor> Type::GetDynamicConstructors(void) const
        {
             auto &handle = database.types[ m_id ].dynamicConstructors;

            std::vector<Constructor> constructors;

            for (auto &constructor : handle)
                constructors.emplace_back( constructor.second );

            return constructors;
        }

        ///////////////////////////////////////////////////////////////////////

        const Constructor &Type::GetConstructor(
            const InvokableSignature &signature
        ) const
        {
            return database.types[ m_id ].GetConstructor( signature );
        }

        ///////////////////////////////////////////////////////////////////////

        const Constructor &Type::GetDynamicConstructor(
            const InvokableSignature &signature
        ) const
        {
            return database.types[ m_id ].GetDynamicConstructor( signature );
        }

        ///////////////////////////////////////////////////////////////////////

        const Constructor &Type::GetArrayConstructor(void) const
        {
            return database.types[ m_id ].arrayConstructor;
        }

        ///////////////////////////////////////////////////////////////////////

        const Destructor &Type::GetDestructor(void) const
        {
            return database.types[ m_id ].destructor;
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Method> Type::GetMethods(void) const
        {
            std::vector<Method> methods;

            auto &handle = database.types[ m_id ].methods;

            for (auto &overload : handle)
            {
                for (auto &method : overload.second)
                {
                    methods.emplace_back( method.second );
                }
            }

            return methods;
        }

        ///////////////////////////////////////////////////////////////////////

        const Method &Type::GetMethod(const std::string &name) const
        {
            return database.types[ m_id ].GetMethod( name );
        }

        ///////////////////////////////////////////////////////////////////////

        const Method &Type::GetMethod(
            const std::string &name, 
            const InvokableSignature &signature
        ) const
        {
            return database.types[ m_id ].GetMethod( name, signature );
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Function> Type::GetStaticMethods(void) const
        {
            std::vector<Function> methods;

            auto &handle = database.types[ m_id ].staticMethods;

            for (auto &overload : handle)
            {
                for (auto &method : overload.second)
                {
                    methods.emplace_back( method.second );
                }
            }

            return methods;
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetStaticMethod(const std::string &name) const
        {
            return database.types[ m_id ].GetStaticMethod( name );
        }

        ///////////////////////////////////////////////////////////////////////

        const Function &Type::GetStaticMethod(
            const std::string &name, 
            const InvokableSignature &signature
        ) const
        {
            return database.types[ m_id ].GetStaticMethod( name, signature );
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Field> Type::GetFields(void) const
        {
            URSINE_TODO( "recursively get base class fields" );

            std::vector<Field> fields;

            auto &handle = database.types[ m_id ].fields;

            for (auto &field : handle)
                fields.emplace_back( field.second );

            return fields;
        }

        ///////////////////////////////////////////////////////////////////////

        const Field &Type::GetField(const std::string &name) const
        {
            return database.types[ m_id ].fields[ name ];
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<Global> Type::GetStaticFields(void) const
        {
            std::vector<Global> fields;

            auto &handle = database.types[ m_id ].staticFields;

            for (auto &field : handle)
                fields.emplace_back( field.second );

            return fields;
        }

        ///////////////////////////////////////////////////////////////////////

        const Global &Type::GetStaticField(const std::string &name) const
        {
            return database.types[ m_id ].staticFields[ name ];
        }

        ///////////////////////////////////////////////////////////////////////

        Json Type::SerializeJson(const Variant &instance, bool invokeHook) const
        {
            UAssert(
                *this == instance.GetType( ),
                "Serializing incompatible variant instance.\n"
                "Got '%s', expected '%s'",
                instance.GetType( ).GetName( ).c_str( ),
                GetName( ).c_str( )
            );

            if (m_isArray)
            {
                Json::array array;

                auto wrapper = instance.GetArray( );
                auto size = wrapper.Size( );

                for (size_t i = 0; i < size; ++i)
                {
                    auto value = wrapper.GetValue( i );

                    array.emplace_back( 
                        value.GetType( ).SerializeJson( value, invokeHook ) 
                    );
                }

                return array;
            }

            if (*this == typeof( bool ))
            {
                return { instance.ToBool( ) };
            }

            if (IsPrimitive( ) || IsEnum( ))
            {
                if (IsFloatingPoint( ) || !IsSigned( ))
                    return { instance.ToDouble( ) };
 
                return { instance.ToInt( ) };
            }

            if (*this == typeof( std::string ))
            {
                return { instance.ToString( ) };
            }
            
            Json::object object { };

            auto &fields = database.types[ m_id ].fields;

            for (auto &field : fields)
            {
                auto value = field.second.GetValue( instance );

                object[ field.first ] = value.SerializeJson( );
            }

			if (invokeHook)
				instance.m_base->OnSerialize( object );

            return object;
        }

        ///////////////////////////////////////////////////////////////////////

        Json Type::SerializeJson(const Variant &instance, SerializationGetterOverride getterOverride, bool invokeHook) const
        {
            UAssert(
                *this == instance.GetType( ),
                "Serializing incompatible variant instance.\n"
                "Got '%s', expected '%s'",
                instance.GetType( ).GetName( ).c_str( ),
                GetName( ).c_str( )
            );

            if (IsArray( ))
            {
                Json::array array;

                auto wrapper = instance.GetArray( );
                auto size = wrapper.Size( );

                for (size_t i = 0; i < size; ++i)
                {
                    auto value = wrapper.GetValue( i );

                    array.emplace_back( 
                        value.GetType( ).SerializeJson( value, invokeHook )
                    );
                }

                return array;
            }

            if (*this == typeof( bool ))
            {
                return { instance.ToBool( ) };
            }

            if (IsPrimitive( ) || IsEnum( ))
            {
                if (IsFloatingPoint( ) || !IsSigned( ))
                    return { instance.ToDouble( ) };
 
                return { instance.ToInt( ) };
            }

            if (*this == typeof( std::string ))
            {
                return { instance.ToString( ) };
            }
            
            Json::object object { };

            auto &fields = database.types[ m_id ].fields;

            for (auto &field : fields)
            {
                auto value = getterOverride( instance, field.second );

                object[ field.first ] = value.SerializeJson( );
            }

			if (invokeHook)
				instance.m_base->OnSerialize( object );

            return object;
        }

        ///////////////////////////////////////////////////////////////////////

        Variant Type::DeserializeJson(const Json &value) const
        {
            auto &ctor = GetConstructor( );

            UAssert( ctor.IsValid( ),
                "Serialization requires a default constructor.\nWith type '%s'.",
                GetName( ).c_str( )
            );

            return DeserializeJson( value, ctor );
        }

        ///////////////////////////////////////////////////////////////////////

        Variant Type::DeserializeJson(const Json &value, const Constructor &ctor) const
        {
            // array types get special care
            if (IsArray( ))
            {
                auto nonArrayType = GetArrayType( );
                auto instance = GetArrayConstructor( ).Invoke( );
                auto wrapper = instance.GetArray( );

                size_t i = 0;

                for (auto &item : value.array_items( ))
                {
                    wrapper.Insert( 
                        i++, 
                        nonArrayType.DeserializeJson( item, ctor ) 
                    );
                }

                return instance;
            }
            // we have to handle all primitive types explicitly
            else if (IsPrimitive( ))
            {
                if (*this == typeof( int ))
                    return { value.int_value( ) };
                else if (*this == typeof( unsigned int ))
                    return { static_cast<unsigned int>( value.number_value( ) ) };
                else if (*this == typeof( bool ))
                    return { value.bool_value( ) };
                else if (*this == typeof( float ))
                    return { static_cast<float>( value.number_value( ) ) };
                else if (*this == typeof( double ))
                    return { value.number_value( ) };
            }
            else if (IsEnum( ))
            {
                return { value.int_value( ) };
            }
            else if (*this == typeof( std::string ))
            {
                return { value.string_value( ) };
            }

            URSINE_TODO( "forward arguments to constructor" );
            auto instance = ctor.Invoke( );

            DeserializeJson( instance, value );

            return instance;
        }

        ///////////////////////////////////////////////////////////////////////

        void Type::DeserializeJson(Variant &instance, const Json &value) const
        {
            auto &fields = database.types[ m_id ].fields;

            for (auto &field : fields)
            {
                auto fieldType = field.second.GetType( );

                UAssert( fieldType.IsValid( ),
                    "Unknown type for field '%s' in base type '%s'. Is this type reflected?",
                    fieldType.GetName( ).c_str( ),
                    GetName( ).c_str( )
                );

				auto &fieldData = value[ field.first ];

				if (!fieldData.is_null( ))
				{
					field.second.SetValue( instance, 
						fieldType.DeserializeJson( fieldData )
					);
				}
            }

            instance.m_base->OnDeserialize( value );
        }
    }
}
