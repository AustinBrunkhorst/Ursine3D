/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** TypeData.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include "MetaManager.h"

#include "Constructor.h"
#include "Destructor.h"

#include "Field.h"
#include "Global.h"

#include "Method.h"
#include "Function.h"

#include "Enum.h"
#include "EnumContainer.h"

namespace ursine
{
    namespace meta
    {
        struct TypeData
        {
            bool isEnum : 1;
            bool isPrimitive : 1;
            bool isSigned : 1;
            bool isFloatingPoint : 1;
            bool isPointer : 1;
            bool isClass : 1;

            MetaManager meta;

            std::string name;

            // enum type

            Enum enumeration;

            // class type

            Type::Set baseClasses;
            Type::Set derivedClasses;

            Constructor arrayConstructor;

            Destructor destructor;

            std::unordered_map<
                InvokableSignature, 
                Constructor
            > constructors;

            std::unordered_map<
                InvokableSignature, 
                Constructor
            > dynamicConstructors;

            std::map<
                std::string, 
                Field
            > fields;

            std::unordered_map<
                std::string, 
                Global
            > staticFields;

            std::unordered_map<
                std::string, 
                InvokableOverloadMap<Method>
            > methods;

            std::unordered_map<
                std::string, 
                InvokableOverloadMap<Function>
            > staticMethods;

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            TypeData(void);
            TypeData(const std::string &name);

            ~TypeData(void);

            void LoadBaseClasses(
                ReflectionDatabase &db, 
                TypeID thisType, 
                const std::initializer_list<Type> &classes
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename ClassType, typename ...Args>
            void AddConstructor(
                Constructor::Invoker invoker, 
                const MetaManager::Initializer &meta, 
                bool isDynamic
            );

            template<typename ClassType>
            void SetArrayConstructor(void);

            const Constructor &GetConstructor(
                const InvokableSignature &signature
            );

            const Constructor &GetDynamicConstructor(
                const InvokableSignature &signature
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename ClassType>
            void SetDestructor(void);

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////
            
            // Method Getter, Method Setter
            template<typename ClassType, typename FieldType, typename MethodGetterType, typename MethodSetterType>
            void AddField(
                const std::string &name, 
                MethodGetterType methodGetter,
                MethodSetterType methodSetter,
                const MetaManager::Initializer &meta
            );

            // Method Getter, Field Setter
            template<typename ClassType, typename FieldType, typename MethodGetterType>
            void AddField(
                const std::string &name, 
                MethodGetterType methodGetter,
                typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
                const MetaManager::Initializer &meta
            );

            // Field Getter, Method Setter
            template<typename ClassType, typename FieldType, typename MethodSetterType>
            void AddField(
                const std::string &name, 
                typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
                MethodSetterType methodSetter,
                const MetaManager::Initializer &meta
            );

            // Field Getter, Field Setter
            template<typename ClassType, typename FieldType>
            void AddField(
                const std::string &name, 
                typename FieldGetter<ClassType, FieldType, false>::Signature fieldGetter,
                typename FieldSetter<ClassType, FieldType, false>::Signature fieldSetter,
                const MetaManager::Initializer &meta
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            // Method Getter, Method Setter
            template<typename ClassType, typename FieldType, typename GetterType, typename SetterType>
            void AddStaticField(
                const std::string &name, 
                GetterType getter,
                SetterType setter,
                const MetaManager::Initializer &meta
            );

            // Method Getter, Field Setter
            template<typename ClassType, typename FieldType, typename GetterType>
            void AddStaticField(
                const std::string &name, 
                GetterType getter,
                FieldType *fieldSetter,
                const MetaManager::Initializer &meta
            );

            // Field Getter, Method Setter
            template<typename ClassType, typename FieldType, typename SetterType>
            void AddStaticField(
                const std::string &name, 
                FieldType *fieldGetter,
                SetterType setter,
                const MetaManager::Initializer &meta
            );

            // Field Getter, Field Setter
            template<typename ClassType, typename FieldType>
            void AddStaticField(
                const std::string &name, 
                FieldType *fieldGetter,
                FieldType *fieldSetter,
                const MetaManager::Initializer &meta
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename MethodType>
            void AddMethod(
                const std::string &name, 
                MethodType method,
                const MetaManager::Initializer &meta
            );

            const Method &GetMethod(
                const std::string &name
            );

            const Method &GetMethod(
                const std::string &name, 
                const InvokableSignature &signature
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<
                typename ClassType, 
                typename FunctionType
            >
            void AddStaticMethod(
                const std::string &name, 
                FunctionType function,
                const MetaManager::Initializer &meta
            );

            const Function &GetStaticMethod(
                const std::string &name
            );

            const Function &GetStaticMethod(
                const std::string &name, 
                const InvokableSignature &signature
            );

            ///////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////

            template<typename EnumType>
            void SetEnum(
                const std::string &name, 
                const typename EnumContainer<EnumType>::Table &table
            );
        };
    }
}

#include "Impl/TypeData.hpp"