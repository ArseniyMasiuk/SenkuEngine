#pragma once
#include "glm\glm.hpp"
#include "SenkuEngine\EventSystem\EventsHandler.h"

namespace Senku
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix;}
		void UpdateCameraPosition(float timeStep);

	private:
		void OnMouseEventHandler(Event& e);
		void ProcessEventWindowResize(Event& e);
		void OnMouseScrollEvent(Event& e);

	private:

		glm::vec3 m_Position;
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		glm::mat4 m_ViewProjectionMatrix;

		float cameraSpeed = 2.5f;
		float yaw = 90.0f;
		float pitch = 0;

		float fildOfView = 45.0f;

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