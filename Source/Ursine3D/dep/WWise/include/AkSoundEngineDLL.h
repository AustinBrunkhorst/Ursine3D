//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

#ifndef AK_SOUND_ENGINE_DLL_H_
#define AK_SOUND_ENGINE_DLL_H_

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>

#include "AkSoundEngineExports.h"

namespace AK
{
    namespace SOUNDENGINE_DLL
    {
        AKSOUNDENGINEDLL_API AKRESULT Init( 
            AkMemSettings *     in_pMemSettings,
            AkStreamMgrSettings *  in_pStmSettings,
            AkDeviceSettings *  in_pDefaultDeviceSettings,
            AkInitSettings *    in_pSettings,
            AkPlatformInitSettings * in_pPlatformSettings,
			AkMusicSettings *	in_pMusicSettings
            );
        AKSOUNDENGINEDLL_API void     Term();

        AKSOUNDENGINEDLL_API void     Tick();

        // File system interface.
		AKSOUNDENGINEDLL_API AKRESULT SetBasePath(
			const AkOSChar*   in_pszBasePath
			);
		AKSOUNDENGINEDLL_API AKRESULT SetBankPath(
			const AkOSChar*   in_pszBankPath
			);
		AKSOUNDENGINEDLL_API AKRESULT SetAudioSrcPath(
			const AkOSChar*   in_pszAudioSrcPath
			);
		AKSOUNDENGINEDLL_API AKRESULT SetLangSpecificDirName(
			const AkOSChar*   in_pszDirName
			);
    }
}

#endif //AK_SOUND_ENGINE_DLL_H_
