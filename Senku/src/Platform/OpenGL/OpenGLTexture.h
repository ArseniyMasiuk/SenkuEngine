#pragma once

#include "SenkuEngine\Renderer\Texture.h"

typedef unsigned int GLenum;

namespace Senku
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
			OpenGLTexture2D(const std::string& path);
			~OpenGLTexture2D();

			virtual void Bind(uint32_t slot = 0) const override;
			virtual void UnBind() const override;

			virtual uint32_t GetWidth() const override { return m_Width; }
			virtual uint32_t GetHeight() const override { return m_Height; }
			virtual const std::string& GetPath() override { return m_Path; }


	private:
		unsigned int m_RendererID;

		uint32_t m_Width, m_Height;
		std::string m_Path;

		GLenum m_InternalFormat, m_DataFormat;
	};
}