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

	bool MaterialInstance::GetTexture(Ref<Texture2D>& texture, TextureType type)
	{
		std::unordered_map<TextureType, Ref<Texture2D>>::iterator found = m_Textures.find(type);
		if (found != m_Textures.end())
		{
			texture = found->second;
			return true;
		}
		return false;
	}

	void MaterialInstance::Bind()
	{
		m_Shader->Bind();
			m_Shader->setUniform1f("u_Material.ambientOclusion", mlt.ambientOclusion);

		if (m_Textures.empty())
		{

			// set general material data to uniforms
			m_Shader->setUniform3fv("u_Material.baseColor", mlt.baseColor);
			//m_Shader->setUniform3fv("u_Material.ambient", mlt.ambient);
			//m_Shader->setUniform3fv("u_Material.diffuse", mlt.diffuse);
			//m_Shader->setUniform3fv("u_Material.specular", mlt.specular);
			//m_Shader->setUniform1f("u_Material.shininess", mlt.specularHighlights);
			m_Shader->setUniform1f("u_Material.metallic", mlt.metallic);
			m_Shader->setUniform1f("u_Material.roughness", mlt.roughness);
			//m_Shader->setUniform1f("u_Material.dissolve", mlt.dissolve);
		}
		else
		{

			int textureMask = 0; // each bit represent binded slot for texture
			int textureSlot = 0;
			for (auto texture : m_Textures)
			{
				textureMask = textureMask | BIT(texture.first);
				m_Shader->setUniform1i(GetUniformNameForTexture(texture.first), textureSlot); // will run through loop and will set all textures to appropriate uniforms and will bind textures to slots
				texture.second->Bind(textureSlot);

				textureSlot++;
			}

			m_Shader->setUniform1i("u_TextureMask", textureMask);
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
