#pragma once
#include "glm\glm.hpp"
#include "SenkuEngine\EventSystem\EventsHandler.h"

namespace Senku
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(glm::vec3 position, float fieldOfView, float aspectRation, float zNear, float zFar);
		~PerspectiveCamera();

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix;}
		
		
		void OnUpdate(float timeStep);

		void Resize(uint32_t width, uint32_t height);

		// todo: Camra class should be recreated since i want to use it in qntity component system and there can be situation that camera is attached to some object
		// so it should have minimal controls or event none of them and script should handle all transformations

		// specially for editor camera

		bool OnEditorCameraUpdate(Event& event);
	private:


	private:
		void RecalculateViewMatrix();

	private:

		glm::vec3 m_Position;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		glm::mat4 m_ViewProjectionMatrix;

		float m_AspectRation;
		float m_Far = 10000.0f;
		float m_Near = 0.1f;
		float m_FieldOfView = 45.0f;

		float yaw = 90.0f;
		float pitch = 0;

		float xoffset;
		float yoffset;
		float delta;
		glm::vec3 origin{ 0.0f };


		float cameraSpeed = 1.5f;
		float lastX, lastY;
		bool firstMouse = true;
	};



	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};



}