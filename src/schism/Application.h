#pragma once
#include <memory>

namespace Schism {
	
	class __declspec(dllexport) Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	std::unique_ptr<Application> CreateApplication();
}
