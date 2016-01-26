/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ModuleFile.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

class Class;
class Global;
class Function;
class Enum;

struct ModuleFile
{
    std::string name;

    std::vector<std::shared_ptr<Class>> classes;
    std::vector<std::shared_ptr<Global>> globals;
    std::vector<std::shared_ptr<Function>> globalFunctions;
    std::vector<std::shared_ptr<Enum>> enums;
};