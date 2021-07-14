#pragma once
#include <numeric>
#include <vector>
#include <string>

#include "schism/System/System.h"

namespace Schism::Gl
{
	enum class ShaderDataType
	{
		None = 0,
		Int,
		Int2,
		Int3,
		Int4,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		//TRACE("Invalid ShaderDataType!");
		return 0;
	};
	
	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			SC_CORE_TRACE("Invalid ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const auto& GetElements() const { return m_Elements; }
		uint32_t GetLength() const
		{
			return m_Length;
		}
	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& el : m_Elements)
			{
				el.Offset = offset;
				offset += el.Size;
				m_Stride += el.Size;
			}
			m_Length = std::accumulate(m_Elements.begin(), m_Elements.end(), 0, [](uint32_t sum, BufferElement& el)
				{
					return el.GetComponentCount() + sum;
				});
		}
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
		uint32_t m_Length;
	};

	class Buffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

}