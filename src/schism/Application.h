#pragma once
#include <memory>

namespace Schism {
	
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	std::unique_ptr<Application> CreateApplication();
}
