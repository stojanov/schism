#include "TextRenderer.h"

#include "glad/glad.h"

namespace Schism
{
	TextRenderer::TextRenderer()
	{
	}

	void TextRenderer::RegisterShader(Resources::ShaderHandle shader) {
		m_Shader = shader;
	}
	
	void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
	{
	}

}
