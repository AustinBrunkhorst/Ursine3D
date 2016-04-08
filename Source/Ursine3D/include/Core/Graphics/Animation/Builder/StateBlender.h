/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** StateBlender.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
** - Hyung Jun Park - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include "Meta.h"

namespace ursine
{
    class StateBlender
    {
    public:
        EditorField(
            std::string currentState,
            GetcurrState,
            SetcurrState
        );

        EditorField(
            std::string futureState,
            GetfutState,
            SetfutState
        );

        //Slider
        EditorMeta(InputRange(0.01f, 5.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
        EditorField(
            float transtionTime,
            GetTransitionTime,
            SetTranstionTime
            );

        //Slider
        // blending starting position of current state
        EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
        EditorField(
            float currtransPos,
            GetcurrTransPosRatio,
            SetcurrTransPosRatio
        );

        //Slider
        // blending end position of future state
        EditorMeta(InputRange(0.0f, 1.0f, 0.01f, "{{value.toPrecision( 2 )}}"))
        EditorField(
            float futtransPos,
            GetfutTransPosRatio,
            SetfutTransPosRatio
        );

        Meta(Enable)
            StateBlender(void);

        const std::string &GetcurrState(void) const;
        void SetcurrState(const std::string& cstate);

        const std::string &GetfutState(void) const;
        void SetfutState(const std::string& fstate);
        
        const float &GetTransitionTime(void) const;
        void SetTranstionTime(const float& transTime);

        const float &GetcurrTransPosRatio(void) const;
        void SetcurrTransPosRatio(const float& tPos);

        const float &GetfutTransPosRatio(void) const;
        void SetfutTransPosRatio(const float& tPos);

        const unsigned int &GetcurrTransFrm(void) const;
        void SetcurrTransFrm(const unsigned int& tFrm);

        const unsigned int &GetfutTransFrm(void) const;
        void SetfutTransFrm(const unsigned int& tFrm);

        const StateBlender *GetStateBlenderByNames(const std::string& currst, const std::string& futst);

    private:
        // name of current state
        std::string m_currState;

        // name of future state
        std::string m_futState;

        //time that takes for transitioning
        float m_transTime;

        // these reperesent the position of the keyframe 
        // ratio = 1 / total keyframe number.
        // if there is 100 frames, ratio 0.01 represents the fist frame
        float m_ctrnsRate;
        float m_ftrnsRate;

        // frame calculated by rates
        unsigned int m_ctrnsFrm;
        unsigned int m_ftrnsFrm;

    } Meta(
        Enable,
        WhiteListMethods,
        EnableArrayType,
        DisplayName("State Blender")
    );
}
