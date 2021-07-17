#pragma once
#include "SharedContext.h"
#include "schism/Core/Events/Event.h"

namespace Schism {
	
	class Application
	{
	public:
		Application(int w, int h, const char* name);
		virtual ~Application();

		void Run();
	private:
		void OnEvent(Event& e);
	protected:
		Core::SharedContextRef m_Ctx;
	};
}
