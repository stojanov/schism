#include "IScene.h"

namespace Schism
{
	IScene::IScene(Core::SharedContextRef ctx, const std::string& name)
		:
		m_Ctx(ctx),
		m_Name(name)
	{
	}

	IScene::~IScene()
	{
		
	}

}