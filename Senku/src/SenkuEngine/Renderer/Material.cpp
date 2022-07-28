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
		m_Texture = texture;
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		m_Shader->setUniform1i("u_Texture", 0); // will run through loop and will set all textures to appropriate uniforms and will bind textures to slots
		m_Texture->Bind(0);
	}
}
