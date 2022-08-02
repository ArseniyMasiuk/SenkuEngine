#include "PrecompiledHeader.h"
#include "ModelLoader.h"
#include "SenkuEngine\Core\Base.h"

#include "glm\glm.hpp"

namespace Senku
{
	void ModelLoader::LoadModel(const std::string & path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			LOG_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return;
		}
		// retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('/'));

		// process ASSIMP's root node recursively
		ProcessNode(scene->mRootNode, scene);
	}
	void ModelLoader::ProcessNode(aiNode * node, const aiScene * scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(mesh, scene);
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	void ModelLoader::ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{

		Model model;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			// tangent and bitangent
			if (mesh->HasTangentsAndBitangents())
			{
				vertex.tangent.x = mesh->mTangents[i].x;
				vertex.tangent.y = mesh->mTangents[i].y;
				vertex.tangent.z = mesh->mTangents[i].z;

				vertex.biTangent.x = mesh->mBitangents[i].x;
				vertex.biTangent.y = mesh->mBitangents[i].y;
				vertex.biTangent.z = mesh->mBitangents[i].z;
			}
			// normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.textureCoordinates = vec;

			}
			else
				vertex.textureCoordinates = glm::vec2(0.0f);



			model.vertecies.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				model.indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//load materials

		aiColor3D  ambient(0.f, 0.f, 0.f);
		if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_AMBIENT, ambient))
			LOG_ERROR("Failed to load material");

		aiColor3D diffuse(0.f, 0.f, 0.f);
		if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
			LOG_ERROR("Failed to load material");

		aiColor3D  specular(0.f, 0.f, 0.f);
		if (AI_SUCCESS != material->Get(AI_MATKEY_COLOR_SPECULAR, specular))
			LOG_ERROR("Failed to load material");

		float shininess;
		if (AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininess))
			LOG_ERROR("Failed to load material");



		model.mlt.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
		model.mlt.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
		model.mlt.specular = glm::vec3(specular.r, specular.g, specular.b);
		model.mlt.specularHighlights = shininess;

		/*
		std::map<aiTextureType, std::vector<s_Texture>> loaded_textures;

		aiTextureType assipTextureTypes[21] =
		{
			aiTextureType_DIFFUSE,
			aiTextureType_SPECULAR,
			aiTextureType_AMBIENT,
			aiTextureType_EMISSIVE,
			aiTextureType_HEIGHT,
			aiTextureType_NORMALS,
			aiTextureType_SHININESS,
			aiTextureType_OPACITY,
			aiTextureType_DISPLACEMENT,
			aiTextureType_LIGHTMAP,
			aiTextureType_REFLECTION,
			aiTextureType_BASE_COLOR,
			aiTextureType_NORMAL_CAMERA,
			aiTextureType_EMISSION_COLOR,
			aiTextureType_METALNESS,
			aiTextureType_DIFFUSE_ROUGHNESS,
			aiTextureType_AMBIENT_OCCLUSION,
			aiTextureType_SHEEN,
			aiTextureType_CLEARCOAT,
			aiTextureType_TRANSMISSION,
			aiTextureType_UNKNOWN
		};

		for (int i = 0; i < 21; i++)
		{
			std::vector<s_Texture> maps = loadMaterialTextures(material, assipTextureTypes[i], MaterialInstance::TextureType::eRoughness);
			loaded_textures.insert(std::pair<aiTextureType, std::vector<s_Texture>>(assipTextureTypes[i], maps));
		}

		// 1. diffuse maps
		std::vector<s_Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, MaterialInstance::TextureType::eRoughness);
		model.textures.insert(model.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<s_Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, MaterialInstance::TextureType::eMetalness);
		model.textures.insert(model.textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<s_Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, MaterialInstance::TextureType::eNormal);
		model.textures.insert(model.textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<s_Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, MaterialInstance::TextureType::eAlbedo);
		model.textures.insert(model.textures.end(), heightMaps.begin(), heightMaps.end());


		//// 1. diffuse maps
		//std::vector<s_Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, MaterialInstance::TextureType::eRoughness);
		//model.textures.insert(model.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		//// 2. specular maps
		//std::vector<s_Texture> specularMaps = loadMaterialTextures(material, aiTextureType_METALNESS, MaterialInstance::TextureType::eMetalness); // not sure about those two and can i treat them as equal
		//model.textures.insert(model.textures.end(), specularMaps.begin(), specularMaps.end());
		//// 3. normal maps
		//std::vector<s_Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, MaterialInstance::TextureType::eNormal);
		//model.textures.insert(model.textures.end(), normalMaps.begin(), normalMaps.end());
		//// 4. height maps
		//std::vector<s_Texture> heightMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, MaterialInstance::TextureType::eAlbedo); // aiTextureType_AMBIENT
		//model.textures.insert(model.textures.end(), heightMaps.begin(), heightMaps.end());

		*/
		models.push_back(model);
		
	}
	
	/*std::vector<s_Texture> ModelLoader::loadMaterialTextures(aiMaterial * mat, aiTextureType type, MaterialInstance::TextureType mateialType)
	{
		std::vector<s_Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				s_Texture texture;
				texture.type = mateialType;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}*/
}