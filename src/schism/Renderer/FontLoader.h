#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.h"
#include "schism/System/Ptr.h"

namespace Schism::Renderer
{
	class FontLoader
	{
	public:
		static void Init();
		static void Shutdown();

		static FT_Library Lib()
		{
			return s_Ft;
		}
	private:
		static FT_Library s_Ft;
	};
	
}
