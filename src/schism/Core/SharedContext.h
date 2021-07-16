#pragma once

#include "Window.h"
#include "schism/System/Ptr.h"
#include "Assets.h"

namespace Schism::Core
{
	struct SharedContext
	{
		Ref<Window> Window;
		Assets GlobalAssets;
	};

	using SharedContextRef = Ref<SharedContext>;

	static const inline SharedContextRef CreateSharedContext(Ref<Window> window)
	{
		Ref<SharedContext> Ctx = MakeRef<SharedContext>();

		Ctx->Window = window;
		return Ctx;
	}
	
}
