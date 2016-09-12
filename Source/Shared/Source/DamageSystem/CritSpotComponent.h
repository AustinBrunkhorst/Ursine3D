#pragma once
/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CritSpotComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include <BoxColliderComponent.h>

struct CritSpot : ursine::ecs::Component
{
    NATIVE_COMPONENT;
public:

    CritSpot(void);
    ~CritSpot(void);

    EditorField(
        bool SearchForObject,
        GetSearch,
        SetSearch
        );
    bool GetSearch(void) const;
    void SetSearch(bool);

protected:
    void OnInitialize(void) override;

private:

    bool m_search;

} Meta(Enable, DisplayName("CritSpot"));
