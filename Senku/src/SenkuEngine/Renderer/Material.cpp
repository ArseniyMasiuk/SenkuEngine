#include "PrecompiledHeader.h"
#include "Material.h"

namespace Senku
{
	Material::Material(Ref<Shader>& shader)
		:m_Shader(shader)
	{

	}

	void Material::SetShader(const Ref<Shader>& shader)
	{
		m_Shader = shader;
	}

	void Material::AddTexture(Ref<Texture2D>& texture, TextureType type)
	{
		ASSERT(!(m_Textures.size() == 32));
		m_Textures.insert(std::make_pair(type, texture));
		m_Texture = texture;
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		int textureSlot = 0;
		for (auto texture : m_Textures)
		{
			m_Shader->setUniform1i(GetUniformNameForTexture(texture.first), textureSlot); // will run through loop and will set all textures to appropriate uniforms and will bind textures to slots
			texture.second->Bind(textureSlot);
			textureSlot++;
		}
	}

	const char* Material::GetUniformNameForTexture(TextureType type)
	{
		switch (type)
		{
		case Senku::Material::eAlbedo: return "u_textureAlbedo";
		case Senku::Material::eNormal: return "u_TextureNormal";
		case Senku::Material::eRoughness: return "u_TextureRoughness";
		case Senku::Material::eMetalness: return "u_TextureMetalness";
		default:
			ASSERT(false);
			return "";
		}
	}
}
