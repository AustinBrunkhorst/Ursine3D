/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** Macros.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#if defined(__REFLECTION_PARSER__)

    #define Meta(...) __attribute__((annotate(#__VA_ARGS__)))

    #define EditorField(expression, getter, setter) Meta(Enable, Getter(#getter), Setter(#setter)) expression
    #define EditorResourceField(resourceType, fieldName) Meta(Enable, ResourceType(typeof(resourceType))) ursine::resources::ResourceReference fieldName

    #if defined(URSINE_WITH_EDITOR)

        #define EditorButton(name, title) Meta(Enable, CreateButton(title)) void name(void);

    #else

        #define EditorButton(name, title)

    #endif

    #define __META_EXTERNAL(type, guid)       \
        typedef type __META_EXTERNAL__##guid; \

    #define _META_EXTERNAL(type, guid) __META_EXTERNAL(type, guid)

    #define MetaExternal(type) _META_EXTERNAL(type, __COUNTER__)

    #define META_OBJECT

#else 

    #define Meta(...)

    #define EditorField(expression, getter, setter)
    #define EditorResourceField(resourceType, fieldName) ursine::resources::ResourceReference fieldName

    #if defined(URSINE_WITH_EDITOR)

        #define EditorButton(name, title) void name(void)

    #else

        #define EditorButton(name, title)

    #endif

    #define MetaExternal(type)

    #define MetaInitialize(initializer)                               \
        {                                                             \
            auto &db = ursine::meta::ReflectionDatabase::Instance( ); \
            initializer;                                              \
        }                                                             \

    // Used in objects to preserve virtual inheritance functionality
    #define META_OBJECT                                  \
        ursine::meta::Type GetType(void) const override  \
        {                                                \
            return typeof( decltype( *this ) );          \
        }                                                \
        ursine::meta::Object *Clone(void) const override \
        {                                                \
            typedef                                      \
            std::remove_const<                           \
                std::remove_reference<                   \
                    decltype( *this )                    \
                >::type                                  \
            >::type ClassType;                           \
            return new ClassType( *this );               \
        }                                                \

#endif