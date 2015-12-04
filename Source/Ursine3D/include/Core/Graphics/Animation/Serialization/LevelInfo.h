/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** LevelInfo.h
**
** Author:
** - Park Hyung Jun - park.hyungjun@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <ISerialize.h>>

namespace ursine
{
	namespace graphics
	{
		namespace ufmt_loader
		{
			struct MeshInLvl
			{
				char name[MAXTEXTLEN];
				SMat4 meshTM;
			};

			class LevelInfo : public ISerialize
			{
			public:
				unsigned int	mmeshlvlCount;
				MeshInLvl*		marrMeshlvls;

				LevelInfo();
				virtual ~LevelInfo();
				void ReleaseData();

				bool SerializeIn(HANDLE hFile);
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};