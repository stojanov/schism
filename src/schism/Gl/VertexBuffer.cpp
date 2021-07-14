#include "VertexBuffer.h"

#include <glad/glad.h>

namespace Schism::Gl
{
	void VertexBuffer::CreateBuffer()
	{
		glCreateBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	VertexBuffer::VertexBuffer(float* vertices, size_t size, const BufferLayout& layout, bool dynamic)
		:
			m_Layout(layout),
			m_Dynamic(dynamic)
	{
		CreateBuffer();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic)
		:
			m_Layout(layout),
			m_Dynamic(dynamic)
	{
		CreateBuffer();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const BufferLayout& layout)
		:
			m_Layout(layout),
			m_Dynamic(true)
	{
		CreateBuffer();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	Ref<VertexBuffer> VertexBuffer::CreateRef(const BufferLayout& layout)
	{
		return MakeRef<VertexBuffer>(layout);
	}

	Ptr<VertexBuffer> VertexBuffer::CreatePtr(const BufferLayout& layout)
	{
		return MakePtr<VertexBuffer>(layout);
	}
	
	Ref<VertexBuffer> VertexBuffer::CreateRef(float* vertices, size_t size, const BufferLayout& layout, bool dynamic)
	{
		return MakeRef<VertexBuffer>(vertices, size, layout, dynamic);
	}

	Ref<VertexBuffer> VertexBuffer::CreateRef(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic)
	{
		return MakeRef<VertexBuffer>(vertices, layout, dynamic);
	}

	Ptr<VertexBuffer> VertexBuffer::CreatePtr(float* vertices, size_t size, const BufferLayout& layout, bool dynamic)
	{
		return MakePtr<VertexBuffer>(vertices, size, layout, dynamic);
	}

	Ptr<VertexBuffer> VertexBuffer::CreatePtr(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic)
	{
		return MakePtr<VertexBuffer>(vertices, layout, dynamic);
	}
	
	void VertexBuffer::SetData(float* vertices, size_t size)
	{
		if (!m_Dynamic) return;
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::UpdateSubData(float* vertices, size_t size)
	{
		// Todo: Finish this
	}
	
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Clear()
	{
		glDeleteBuffers(1, &m_BufferID);
	}
}
