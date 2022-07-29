#include "PrecompiledHeader.h"
#include "Material.h"

namespace Senku
{
	MaterialInstance::MaterialInstance(Ref<Shader>& shader)
		:m_Shader(shader)
	{

	}

	void MaterialInstance::SetShader(const Ref<Shader>& shader)
	{
		m_Shader = shader;
	}

	void MaterialInstance::AddTexture(Ref<Texture2D>& texture, TextureType type)
	{
		ASSERT(!(m_Textures.size() == 32));
		m_Textures.insert(std::make_pair(type, texture));
	}

	void MaterialInstance::Bind()
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

	const char* MaterialInstance::GetUniformNameForTexture(TextureType type)
	{
		switch (type)
		{
		case Senku::MaterialInstance::eAlbedo: return "u_textureAlbedo";
		case Senku::MaterialInstance::eNormal: return "u_TextureNormal";
		case Senku::MaterialInstance::eRoughness: return "u_TextureRoughness";
		case Senku::MaterialInstance::eMetalness: return "u_TextureMetalness";
		default:
			ASSERT(false);
			return "";
		}
	}
}
