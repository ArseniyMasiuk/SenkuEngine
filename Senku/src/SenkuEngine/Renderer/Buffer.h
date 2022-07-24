#pragma once
#include "SenkuEngine\Core\Base.h"


namespace Senku
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const  = 0;

		static Ref<VertexBuffer> Create(float * vertecies, uint32_t size);

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const  = 0;
		virtual void Unbind() const  = 0;

		static Ref<IndexBuffer> Create(uint32_t* indecies, uint32_t count);

	};
}