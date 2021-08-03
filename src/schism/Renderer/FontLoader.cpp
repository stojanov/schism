#include "FontLoader.h"



#include "schism/System/Log.h"

namespace Schism::Renderer
{
	void FontLoader::Init()
	{
		if (FT_Init_FreeType(&s_Ft))
		{
			SC_CORE_CRITICAL("Cannot load font library");
		}
	}

	void FontLoader::Shutdown()
	{
		FT_Done_FreeType(s_Ft);
	}

}