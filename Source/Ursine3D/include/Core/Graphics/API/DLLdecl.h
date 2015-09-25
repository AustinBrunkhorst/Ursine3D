/* Start Header ---------------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
=============================================================================*/
/*!
File Name:      DLLdecl.h
Module:         Graphics
Purpose:        declaration for importing from dll
Language:       C++

Project:        Graphics Prototype
Author:         Matt Yan, m.yan@digipen.edu
*/
/*- End Header --------------------------------------------------------------*/

#pragma once

#ifdef DLL_EXPORT
#define __declspec(dllexport) 
#else
#define __declspec(dllimport) 
#endif