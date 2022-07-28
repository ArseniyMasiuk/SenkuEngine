#pragma once
#include "Shader.h"
#include "Texture.h"


namespace Senku
{

	class Material
	{
		// base things
		// Wavefront Material Template Library (MTL)
		// https://www.loc.gov/preservation/digital/formats/fdd/fdd000508.shtml
		struct GeneralMLT
		{
			glm::vec3 baseColor{ 1.0f };
			glm::vec3 ambinet{ 1.0f };			// Ka: specifies ambient color, to account for light that is scattered about the entire scene [see Wikipedia entry for Phong Reflection Model] using values between 0 and 1 for the RGB components.
			glm::vec3 diffuse{ 1.0f };			// Kd: specifies diffuse color, which typically contributes most of the color to an object. In this example, Kd represents a grey color, which will get modified by a colored texture map specified in the map_Kd statement
			glm::vec3 specular{ 1.0f };			// Ks: specifies specular color, the color seen where the surface is shiny and mirror-like
			float specularHighlights = 1.0f;	// Ns: defines the focus of specular highlights in the material. Ns values normally range from 0 to 1000, with a high value resulting in a tight, concentrated highlight.
			float refraction = 1.0;				// Ni: defines the optical density (aka index of refraction) in the current material. The values can range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.
			float dissolve = 1.0f;				//d: specifies a factor for dissolve, how much this material dissolves into the background. A factor of 1.0 is fully opaque. A factor of 0.0 is completely transparent.
			// int illumination; // docs says that this is ignored but i wilk keep it for now maybe it will be in use later
		};

	public:

		enum TextureType : uint32_t { eAlbedo = 1, eNormal, eRoughness, eMetalness/*, eHeight, eAmbientOclustion, eRefraction*/ }; // probably will not need all of the but it will be good to have all of them here

	public:
		Material(Ref<Shader>& shader);

		void SetShader(const Ref<Shader>& shader);
		const Ref<Shader> GetShader() { return m_Shader; };

		void AddTexture(Ref<Texture2D>& texture, TextureType type);



		void Bind(); // should set up all material data to shader uniforms 

	private:

		std::unordered_map<TextureType, Ref<Texture2D>> m_Textures;
		Ref<Texture2D> m_Texture;
		Ref<Shader>& m_Shader;

		// for easier set up
	public:
		GeneralMLT mlt;


	private:
		const char* GetUniformNameForTexture(TextureType type);
	};
}
