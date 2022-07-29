#include "PrecompiledHeader.h"
#include "OpenGLTexture.h"
#include "DebugAsist.h"
#include "stb_image.h"


namespace Senku
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string & path)
		:m_Path(path), m_Width(0), m_Height(0)
	{

		int width, height, channels;

		//stbi_set_flip_vertically_on_load(1);

		stbi_uc *buffer;
		buffer = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!buffer)
			LOG_ERROR("Cant load texture by path {0}", path);
		else
		{
			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			ASSERT(internalFormat & dataFormat);

			GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
			GLCall(glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height));

			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

			GLCall(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, buffer));

			//GLCall(glBindTexture(GL_TEXTURE_2D, 0));

			stbi_image_free(buffer);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void OpenGLTexture2D::Bind(uint32_t slot/* = 0*/) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void OpenGLTexture2D::UnBind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
