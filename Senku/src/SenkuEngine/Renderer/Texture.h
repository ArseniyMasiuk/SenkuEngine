#pragma once
#include "SenkuEngine\Core\Base.h"

namespace Senku
{
	class Texture
	{
	public:
		~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetRendererID() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual const std::string& GetPath() = 0;


	};

	class Texture2D : public Texture
	{
	public:

		static Ref<Texture2D> Create(const std::string& path);
	};
}