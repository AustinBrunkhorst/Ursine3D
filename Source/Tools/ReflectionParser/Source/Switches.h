/* ----------------------------------------------------------------------------
** Team Bear King
** © 201x DigiPen Institute of Technology, All Rights Reserved.
**
** Switches.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <string>

#define DEFINE_SWITCH(name, value)                   \
    const std::string kSwitch##name = value;         \
    const std::string kSwitch##name##Shorthand = ""; \

#define DEFINE_SWITCH_FULL(name, value, shorthand)              \
    const std::string kSwitch##name = value;                    \
    const std::string kSwitch##name##Shorthand = "," shorthand; \

#define SWITCH_OPTION(name)                                 \
    ((kSwitch##name) + (kSwitch##name##Shorthand)).c_str( ) \

DEFINE_SWITCH_FULL( Help,                      "help",           "h" );
DEFINE_SWITCH_FULL( TargetName,                "target-name",    "t" );
DEFINE_SWITCH_FULL( SourceRoot,                "source-root",    "r" );
DEFINE_SWITCH_FULL( InputSource,               "in-source",      "i" );
DEFINE_SWITCH_FULL( ModuleHeaderFile,          "module-header",  "m" );
DEFINE_SWITCH_FULL( OutputModuleSource,        "out-source",     "s" );
DEFINE_SWITCH_FULL( OutputModuleFileDirectory, "out-dir",        "c" );
DEFINE_SWITCH_FULL( TemplateDirectory,         "tmpl-directory", "d" );
DEFINE_SWITCH_FULL( PrecompiledHeader,         "pch",            "p" );
DEFINE_SWITCH_FULL( CompilerFlags,             "flags",          "f" );
