#pragma once

#include <cstdint>
#include <glad/glad.h>

#include "schism/System/System.h"
#include "Buffer.h"

namespace Schism::Gl
{
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const BufferLayout& layout);
		VertexBuffer(float* vertices, size_t size, const BufferLayout& layout, bool dynamic = true);
		VertexBuffer(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic = true);
		virtual ~VertexBuffer();

		static Ref<VertexBuffer> CreateRef(const BufferLayout& layout);
		static Ptr<VertexBuffer> CreatePtr(const BufferLayout& layout);
		static Ref<VertexBuffer> CreateRef(float* vertices, size_t size, const BufferLayout& layout, bool dynamic = true);
		static Ref<VertexBuffer> CreateRef(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic = true);
		static Ptr<VertexBuffer> CreatePtr(float* vertices, size_t size, const BufferLayout& layout, bool dynamic = true);
		static Ptr<VertexBuffer> CreatePtr(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic = true);
		
		const BufferLayout& GetLayout() { return m_Layout; };

		void SetData(float* vertices, size_t size);
		void UpdateSubData(float* vertices, size_t size);

		template<typename T>
		void SetData(std::vector<T>& vertices, size_t count)
		{
			if (!m_Dynamic) return;
			Bind();
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(T), &vertices[0], GL_DYNAMIC_DRAW);
		}
		
		void Bind() const override;
		void Unbind() const override;

		void Clear();
	private:
		void CreateBuffer();

		bool m_Dynamic{ false };
		unsigned m_BufferID;
		BufferLayout m_Layout;
	};
}
