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
				MeshInLvl		marrMeshlvls;

				LevelInfo();
				virtual ~LevelInfo();

				bool SerializeIn(HANDLE hFile);
				bool SerializeOut(HANDLE hFile);
			};
		};
	};
};