#pragma once

#include "TypeConfig.h"
#include "InvokableConfig.h"
#include "ArgumentConfig.h"

#include <string>
#include <vector>
#include <set>

namespace ursine
{
    namespace meta
    {
        class Variant;
        class Enum;
        class Constructor;
        class Destructor;
        class Field;
        class Global;
        class Method;
        class Function;
        class Argument;

        class Type
        {
        public:
            typedef std::vector<Type> List;
            typedef std::set<Type> Set;

            static const TypeID Invalid = 0;

            Type(void);
            Type(const Type &rhs);
            Type(TypeID id);

            operator bool(void) const;

            Type &operator=(const Type &rhs);

            bool operator<(const Type &rhs) const;
            bool operator>(const Type &rhs) const;
            bool operator<=(const Type &rhs) const;
            bool operator>=(const Type &rhs) const;
            bool operator==(const Type &rhs) const;
            bool operator!=(const Type &rhs) const;

            TypeID GetID(void) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets all types registered
            static List GetTypes(void);

            // Gets a type based on the qualified string name
            static Type Get(const char *name);

            // Gets a type by deducing the type of an object
            template<typename T>
            static Type Get(T &&obj);

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets all registered global variables
            static std::vector<Global> GetGlobals(void);

            // Gets a global variable with the specified name
            static const Global &GetGlobal(const std::string &name);

            // Gets all registered global functions
            static std::vector<Function> GetGlobalFunctions(void);

            // Gets a global function with the specified name, and first
            // available overload
            static const Function &GetGlobalFunction(const std::string &name);

            // Gets a global function with the specified name, and overload signature
            static const Function &GetGlobalFunction(const std::string &name, const InvokableSignature &signature);

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Compares two type lists. Returns true if "a" is identical to "b"
            static bool ListsEqual(const List &a, const List &b);

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Determines if this is a valid type
            bool IsValid(void) const;

            // Determines if this type is primitive (int, bool, char, etc)
            bool IsPrimitive(void) const;

            // Determines if this type is an enumeration
            bool IsEnum(void) const;

            // Determines if this type is a pointer
            bool IsPointer(void) const;

            // Determines if this type is a class
            bool IsClass(void) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets the human readable name for this type
            const std::string &GetName(void) const;

            // Instantiates an instance of this type with the given constructor signature
            // NOTE: it is much faster to cache the appropriate constructor first, then
            // call Invoke( ) manually
            Variant CreateVariadic(const ArgumentList &arguments) const;

            // Instantiates an instance of this type with the given dynamic constructor signature
            // NOTE: it is much faster to cache the appropriate constructor first, then
            // call Invoke( ) manually
            Variant CreateDynamicVariadic(const ArgumentList &arguments) const;

            // Instantiates an instance of this type with the given constructor signature
            // NOTE: it is much faster to cache the appropriate constructor first, then
            // call Invoke( ) manually
            template<typename ...Args>
            Variant Create(Args &&...args) const;

            // Instantiates an instance of this type with the given dynamic constructor signature
            // NOTE: it is much faster to cache the appropriate constructor first, then
            // call Invoke( ) manually
            template<typename ...Args>
            Variant CreateDynamic(Args &&...args) const;

            // Deconstructs the given object instance
            void Destroy(Variant &instance) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets the decayed version of this type (no qualifiers like const, and no pointer)
            Type GetDecayedType(void) const;

            // Gets the enumeration representing this type, assuming it's an enum type
            const Enum &GetEnum(void) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Determines if this type derives from the specified runtime type
            bool DerivesFrom(const Type &other) const;

            // Determines if this type derives from the specified static type
            template<typename T>
            bool DerivesFrom(void) const;

            // Gets all base classes for this type
            const Set &GetBaseClasses(void) const;

            // Gets all classes that derive from this class type
            const Set &GetDerivedClasses(void) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets all constructors for this type assuming it's a class type
            std::vector<Constructor> GetConstructors(void) const;

            // Gets all dynamic constructors for this type assuming it's a class type
            std::vector<Constructor> GetDynamicConstructors(void) const;

            // Gets a constructor for this type with the specified argument signature
            const Constructor &GetConstructor(const InvokableSignature &signature = InvokableSignature( )) const;

            // Gets a dynamic constuctor for this type with the specified argument signature
            const Constructor &GetDynamicConstructor(const InvokableSignature &signature = InvokableSignature( )) const;

            // Gets the destructor for this type assuming it's a class type
            const Destructor &GetDestructor(void) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets all methods for this type, assuming it's a class type
            std::vector<Method> GetMethods(void) const;

            // Gets a specific method for this type, assuming it's a class type.
            // If the method doesn't exist, an invalid method is returned.
            // If the method exists, but has overloads, the first declared
            // overload is returned.
            const Method &GetMethod(const std::string &name) const;

            // Gets a specific method for this type, with the given 
            // overloaded signature, assuming it's a class type.
            // If the overload doesn't exist, an invalid method is returned.
            const Method &GetMethod(const std::string &name, const InvokableSignature &signature) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets all static methods for this type, assuming it's a class type
            std::vector<Function> GetStaticMethods(void) const;

            // Gets a specific static method for this type, assuming it's a class type.
            // If the method doesn't exist, an invalid function is returned.
            // If the method exists, but has overloads, the first declared
            // overload is returned.
            const Function &GetStaticMethod(const std::string &name) const;

            // Gets a specific static method for this type, with the given 
            // overloaded signature, assuming it's a class type.
            // If the overload doesn't exist, an invalid function is returned.
            const Function &GetStaticMethod(const std::string &name, const InvokableSignature &signature) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets all fields for this type assuming it's a class type
            std::vector<Field> GetFields(void) const;

            // Gets a specific field for this type, assuming it's a class type
            const Field &GetField(const std::string &name) const;

            ////////////////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////////////////

            // Gets all static fields for this type assuming it's a class type
            std::vector<Global> GetStaticFields(void) const;

            // Gets a specific static field for this type, assuming it's a class type
            const Global &GetStaticField(const std::string &name) const;

        private:
            friend class std::allocator<Type>;

            friend struct TypeData;

            friend class Variant;
            friend class Argument;
            friend class Enum;
            friend class EnumBase;
            friend class Constructor;
            friend class Destructor;
            friend class Method;
            friend class Field;
            friend class Function;
            friend class Global;

            TypeID m_id;
        };
    }
}

#include "Impl/Type.hpp"