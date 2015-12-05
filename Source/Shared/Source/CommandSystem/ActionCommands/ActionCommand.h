/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <memory>

#include "Command.h"

class ActionCommandBase
{
public:
    virtual ~ActionCommandBase() {};
    virtual bool Acting(void) { return false; };
    virtual std::shared_ptr<Command> CreateCommand() { return std::shared_ptr<Command>(nullptr); }
};

template<typename T>
class ActionCommand
    : public ActionCommandBase
{
public:
    virtual ~ActionCommand();
    virtual bool Acting(void) override;
    virtual std::shared_ptr<Command> CreateCommand() override;
};

/////////////////////////////////////////////////////////////////////

// these methods are NEVER suppposed to be called
template<typename T>
ActionCommand<T>::~ActionCommand() {}

template<typename T>
bool ActionCommand<T>::Acting()
{
    return false;
}

template<typename T>
std::shared_ptr<Command> ActionCommand<T>::CreateCommand()
{
    return std::shared_ptr<Command>(nullptr);
}
