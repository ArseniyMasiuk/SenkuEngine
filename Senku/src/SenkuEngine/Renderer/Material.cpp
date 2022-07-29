#include "PrecompiledHeader.h"
#include "Material.h"
#include "SenkuEngine\Core\Base.h"


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

		// set general material data to uniforms
		m_Shader->setUniform3fv("u_Material.baseColor", mlt.baseColor);
		m_Shader->setUniform1f("u_Material.dissolve",mlt.dissolve);

		int textureMask = 0; // each bit represent binded slot for texture
		int textureSlot = 0;
		for (auto texture : m_Textures)
		{
			m_Shader->setUniform1i(GetUniformNameForTexture(texture.first), textureSlot); // will run through loop and will set all textures to appropriate uniforms and will bind textures to slots
			texture.second->Bind(textureSlot);

			textureMask = textureMask | BIT(textureSlot);
			textureSlot++;
		}

		m_Shader->setUniform1i("u_TextureMask", textureMask);
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
