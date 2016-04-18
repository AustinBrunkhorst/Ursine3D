/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SoundFrameClient.cpp
**
** Author:
** - Chad George - c.george@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include <UrsinePrecompiled.h>
#include "SoundFrameClient.h"

namespace ursine
{

    SoundFrameClient::SoundFrameClient( ) : m_pSoundFrame(nullptr)
    {
        Create(this, &m_pSoundFrame);

        if ( m_pSoundFrame )
            m_pSoundFrame->Connect( );
    }

    SoundFrameClient::~SoundFrameClient( )
    {
        if ( m_pSoundFrame )
            m_pSoundFrame->Release( );
    }

} // ursine namespace