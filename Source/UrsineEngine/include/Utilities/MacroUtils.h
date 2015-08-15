/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** MacroUtils.h
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#define STR(x) #x
#define STRINGIFY(x) STR(x)

#define __MESSAGE(text) __pragma( message(__FILE__ "(" STRINGIFY(__LINE__) ")" text) ) 

// Outputs a warning during compile time
#define URSINE_WARNING(text) __MESSAGE( " : Warning: " ##text )

// Outputs an error during compile time
#define URSINE_ERROR(text) __MESSAGE( " : Error: " ##text )

// Outputs a message during compile time
#define URSINE_MESSAGE(text) __MESSAGE( ": " ##text )

#ifdef URSINE_TODO_AS_WARNINGS

// Outputs a TODO message during compile time
#define URSINE_TODO(text) __MESSAGE( " : Warning: TODO: " ##text )

#else

// Outputs a TODO message during compile time
#define URSINE_TODO(text) __MESSAGE( ": TODO: " ##text )

#endif
