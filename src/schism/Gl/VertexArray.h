#pragma once

#include <glad/glad.h>

#include "Buffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "schism/System/System.h"

namespace Schism::Gl
{
	static GLenum OpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		SC_ASSERT(false, "Invalid shader data type")
	}

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;
		
		void AddVertexBuffer(const Ref<VertexBuffer>& buf);
		void SetIndexBuffer(const Ref<IndexBuffer>& buffer);
	private:
		uint32_t m_ID;
		uint32_t m_BufferIndex{ 0 };
		uint32_t m_Stride;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
