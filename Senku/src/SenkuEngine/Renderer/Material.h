#pragma once
#include "Shader.h"
#include "Texture.h"


namespace Senku
{

	class Material
	{
	public:

		enum TextureType : uint32_t { eAlbedo = 1 };

	public:
		Material(Ref<Shader>& shader);


		void SetShader(const Ref<Shader>& shader);
		const Ref<Shader> GetShader() { return m_Shader; };

		void AddTexture(Ref<Texture2D>& texture, TextureType type);

		void Bind(); // should set up all material data to shader uniforms 

	private:

		Ref<Texture2D> m_Texture;
		Ref<Shader>& m_Shader;
	};
}
