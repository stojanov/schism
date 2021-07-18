#pragma once

#include "Window.h"
#include "schism/System/Ptr.h"
#include "Assets.h"

namespace Schism::Core
{
	struct SceneManagerCallbacks
	{
		std::function<void(const std::string&)> Switch{ nullptr };
		std::function<void(const std::string&)> Destroy{ nullptr };
	};
	
	struct SharedContext
	{
		Ref<Window> Window;
		Assets GlobalAssets;
		SceneManagerCallbacks SceneManager;
	};

	using SharedContextRef = Ref<SharedContext>;

	static const SharedContextRef CreateSharedContext(Ref<Window> window)
	{
		Ref<SharedContext> Ctx = MakeRef<SharedContext>();

		Ctx->Window = window;
		return Ctx;
	}
	
}
