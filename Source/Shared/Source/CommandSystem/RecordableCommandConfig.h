/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RecordableCommandConfig.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#define RECORDABLE_COMMAND                 \
private:                                   \
    static ursine::uint typeID;            \
public:                                    \
    ursine::uint GetTypeID(void) override; \
private:                                   \

#define RECORDABLE_COMMAND_DEFINITION(type)              \
ursine::uint type::typeID = RecordableCommand::GetID( ); \
ursine::uint type::GetTypeID(void)                       \
{                                                        \
    return typeID;                                       \
}                                                        \

namespace { }