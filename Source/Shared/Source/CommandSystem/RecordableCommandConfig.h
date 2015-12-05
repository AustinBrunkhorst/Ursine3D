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