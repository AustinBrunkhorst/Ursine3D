/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ActionCommand.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once


class ActionCommandBase
{
public:
    virtual ~ActionCommandBase() {};
    
    virtual void ProcessCommands(void) { };

protected:
    
};

template<typename T>
class ActionCommand
    : public ActionCommandBase
{
public:
    virtual ~ActionCommand();
    virtual bool Acting(void);
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
