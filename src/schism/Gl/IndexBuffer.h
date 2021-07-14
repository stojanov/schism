#pragma once

#include <glad/glad.h>

#include "schism/System/System.h"
#include "Buffer.h"

namespace Schism::Gl
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer();
		IndexBuffer(uint32_t* indices, uint32_t count);
		IndexBuffer(std::vector<uint32_t>& indices);
		
		virtual ~IndexBuffer();
		
		static Ref<IndexBuffer> CreateRef(uint32_t* indices, uint32_t count);
		static Ptr<IndexBuffer> CreatePtr(uint32_t* indices, uint32_t count);
		static Ref<IndexBuffer> CreateRef();
		static Ptr<IndexBuffer> CreatePtr();

		void SetData(uint32_t* indices, uint32_t count) const;
		void SetData(std::vector<uint32_t>& indices, uint32_t count) const;
		
		void Bind() const override;
		void Unbind() const override;
		void Clear() const;
	private:
		void CreateBuffer()
		{
			glCreateBuffers(1, &m_BufferID);
			// Used GL_ARRAY_BUFFER so i can load data without an active VAO
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		}

		unsigned m_BufferID;
		uint32_t m_Count;
	};
}
