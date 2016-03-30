/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** StateBlender.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "StateBlender.h"

namespace ursine
{
    StateBlender::StateBlender(void)
        : m_currState( "" )
        , m_futState( "" )
        , m_ctrnsRate( 1.f )
        , m_ftrnsRate( 0.f )
        , m_ctrnsFrm( 0 )
        , m_ftrnsFrm( 0 ) { }

    const std::string &StateBlender::GetcurrState(void) const
    {
        return m_currState;
    }

    void StateBlender::SetcurrState(const std::string &cstate)
    {
        m_currState = cstate;
    }

    const std::string &StateBlender::GetfutState(void) const
    {
        return m_futState;
    }

    void StateBlender::SetfutState(const std::string &fstate)
    {
        m_futState = fstate;
    }

    const float &StateBlender::GetcurrTransPosRatio(void) const
    {
        return m_ctrnsRate;
    }

    void StateBlender::SetcurrTransPosRatio(const float &tPos)
    {
        m_ctrnsRate = tPos;
    }

    const float &StateBlender::GetfutTransPosRatio(void) const
    {
        return m_ftrnsRate;
    }

    void StateBlender::SetfutTransPosRatio(const float &tPos)
    {
        m_ftrnsRate = tPos;
    }

    const unsigned int &StateBlender::GetcurrTransFrm(void) const
    {
        return m_ctrnsFrm;
    }

    void StateBlender::SetcurrTransFrm(const unsigned int &tFrm)
    {
        m_ctrnsFrm = tFrm;
    }

    const unsigned int &StateBlender::GetfutTransFrm(void) const
    {
        return m_ftrnsFrm;
    }

    void StateBlender::SetfutTransFrm(const unsigned int &tFrm)
    {
        m_ftrnsFrm = tFrm;
    }

    const StateBlender *StateBlender::GetStateBlenderByNames(const std::string &currst, const std::string &futst)
    {
        if (currst == m_currState && futst == m_futState)
            return this;

        return nullptr;
    }
}
