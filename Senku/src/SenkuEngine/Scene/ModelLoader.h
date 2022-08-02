#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "SenkuEngine\Renderer\Material.h"

namespace Senku
{
	struct Vertex
	{
		glm::vec3 position{ 0.0f };
		glm::vec3 normal{ 0.0f };
		glm::vec2 textureCoordinates{ 0.0f };
		glm::vec3 tangent;
		glm::vec3 biTangent;
	};

	/*struct s_Texture
	{
		MaterialInstance::TextureType type;
		std::string path;

	};*/

	struct Model
	{
		std::vector<Vertex> vertecies;
		std::vector<uint32_t> indices;
		MaterialInstance::GeneralMLT mlt;
		//std::vector<s_Texture> textures;
	};

	class ModelLoader
	{
	public:
		/* maybe it sould be cached to some buffer
		maybe even to some binary file especially for vertecies.
		just store them in form of data that sould go to vertex and index buffers ??? 
		probably will add it later

		also there is possibility to load several meshes that are part of full model
		so we shuld handle it too
		but i will add it later
		todo: add loading several meshes to same model
		*/
		
		void LoadModel(const std::string & path);


		const std::vector<Model>& GetModels() { return models; }


	private:
		void ProcessNode(aiNode * node, const aiScene * scene);
		void ProcessMesh(aiMesh * mesh, const aiScene * scene);

		//std::vector<s_Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, MaterialInstance::TextureType mateialType);


		std::vector<Model> models;
		std::string directory;
	};

}