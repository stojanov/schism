#include "schism/Interfaces/IScene.h"
#include "schism/Renderer/OrthographicCamera.h"
#include "schism/Core/Assets.h"
#include "BoardManager.h"
#include "Engine.h"


using namespace Schism;

namespace Chess
{
	class Chess : public IScene
	{
	public:
		Chess(Core::SharedContextRef ctx, const std::string& name);
		~Chess();

		void OnAttach() override;
		void OnDetach() override;
		void OnPause() override;
		void OnResume() override;
		void OnUpdate(Timestep ts) override;
		void OnDraw() override;
		void OnSystemEvent(Event& e) override;
	private:
		Core::Assets m_Assets;
		BoardManager m_BoardManager;
		Engine m_Engine;
		Renderer::OrthographicCamera m_Camera;
	};
}
