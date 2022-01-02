#include "schism/Interfaces/IScene.h"
#include "schism/Renderer/OrthographicCamera.h"
#include "schism/Core/Assets.h"
#include "schism/Game/Entity.h"


using namespace Schism;


class Chess: public IScene
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
	Entity m_Piece;
	Core::Assets Assets;
	Renderer::OrthographicCamera m_Camera;
};