#pragma once

namespace Schism {
	
	class Application
	{
	public:
		Application(int w, int h, const char* name);
		virtual ~Application();

		void Run();
	protected:
	};
}
