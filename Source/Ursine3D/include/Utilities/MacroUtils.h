/* ----------------------------------------------------------------------------
** Team Bear King
** ?201x DigiPen Institute of Technology, All Rights Reserved.
**
** MacroUtils.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#define __STRINGIFY(x) #x
#define STRINGIFY(x) __STRINGIFY(x)

#define __WIDEN(x) L ## x
#define WIDEN(x) __WIDEN(x)

#if defined(__REFLECTION_PARSER__)

#define __MESSAGE(text)

#else

#define __MESSAGE(text) __pragma( message(__FILE__ "(" STRINGIFY(__LINE__) ")" text) )

#endif

#if defined(CONFIG_RELEASE)

#define URSINE_DISPLAY_TODOS

#endif

// Outputs a warning during compile time
#define URSINE_WARNING(text) __MESSAGE( " : Warning: " ##text )

// Outputs an error during compile time
#define URSINE_ERROR(text) __MESSAGE( " : Error: " ##text )

// Outputs a message during compile time
#define URSINE_MESSAGE(text) __MESSAGE( ": " ##text )

#if defined(URSINE_TODO_AS_WARNINGS)

// Outputs a TODO message during compile time
#define URSINE_TODO(text) __MESSAGE( " : Warning: TODO: " ##text )

#elseif defined(URSINE_DISPLAY_TODOS)

// Outputs a TODO message during compile time
#define URSINE_TODO(text) __MESSAGE( ": TODO: " ##text )

#else

#define URSINE_TODO(text)

#endif
