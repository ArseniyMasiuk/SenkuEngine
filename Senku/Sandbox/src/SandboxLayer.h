#pragma once
#include "SenkuEngine.h"

#include "glm\ext\quaternion_transform.hpp"
#include "glm\ext\matrix_transform.hpp"
#include "glm\gtx\quaternion.hpp"

struct Transform
{
	glm::vec3 m_Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 GetMatrix()
	{
		glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

		return glm::translate(glm::mat4(1.0f), m_Translation)
			* rotation
			* glm::scale(glm::mat4(1.0f), m_Scale);
	}
};

class SandBoxLayer : public Senku::Layer
{
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	Senku::Ref<Senku::FrameBuffer> m_FrameBuffer;

	Senku::Ref<Senku::PerspectiveCamera> m_Camera;
	
	Senku::Ref<Senku::Shader> m_Shader;
	Senku::Ref<Senku::MaterialInstance> m_Material;
	std::vector<std::pair<Transform, Senku::Ref<Senku::VertexArray>>> m_VertexArrays;
	std::vector<std::string> m_Names;
	Senku::Ref<Senku::Texture2D> texture;

	
	std::vector<glm::mat4> transform;
	


	//ImGui controls
	bool m_ShowDemo = false;


private:

	void ShowMeshesTree();
	void ShowMeshProperties(unsigned int index);


public:
	SandBoxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(/*Timestep ts*/float timeStep) override;
	
	virtual bool OnEvent(Senku::Event& event) override;

	virtual void OnImGuiRender() override;
};