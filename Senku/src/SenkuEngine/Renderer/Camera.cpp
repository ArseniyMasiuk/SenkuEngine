#include "PrecompiledHeader.h"
#include "Camera.h"
#include "SenkuEngine\Core\Application.h"
#include "glm\ext\matrix_transform.hpp"
#include "glm\ext\matrix_clip_space.hpp"

#include "SenkuEngine\EventSystem\MouseEvent.h"
#include "SenkuEngine\Core\Input.h"

namespace Senku
{
	PerspectiveCamera::PerspectiveCamera(float fieldOfView, float aspectRation, float zNear, float zFar)
		:m_FieldOfView(fieldOfView), m_AspectRation(aspectRation), m_Far(zFar), m_Near(zNear)
	{
		m_Position = glm::vec3(0.0f, 0.0f, -3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		m_ViewMatrix = glm::mat4(1.0f);

		RecalculateViewMatrix();

		//EventsHandler::GetInstance().SubscribeForEvent(EventCategory(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput),
		//	BIND_EVENT_FN(PerspectiveCamera::OnMouseEventHandler));

		//EventsHandler::GetInstance().SubscribeForEvent(EventCategory(EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput), BIND_EVENT_FN(PerspectiveCamera::OnMouseScrollEvent));
		//EventsHandler::GetInstance().SubscribeForEvent(EventCategory::EventCategoryApplication, BIND_EVENT_FN(PerspectiveCamera::ProcessEventWindowResize));
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::UpdateCameraPosition(float timeStep)
	{
		float delta = cameraSpeed * timeStep;

		if(Input::IsKeyPressed(Key::LeftShift))
			delta = (cameraSpeed + 3) * timeStep;

		if (Input::IsKeyPressed(Key::Left))
			cameraSpeed = 2.5f;

		if (Input::IsKeyPressed(Key::Up))
			cameraSpeed += 2;

		if (Input::IsKeyPressed(Key::Down))
			cameraSpeed -= 2;

		if (Input::IsKeyPressed(Key::W))
			m_Position += delta * cameraFront;

		if (Input::IsKeyPressed(Key::S))
			m_Position -= delta * cameraFront;

		if (Input::IsKeyPressed(Key::A))
			m_Position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;

		if (Input::IsKeyPressed(Key::D))
			m_Position += glm::normalize(glm::cross(cameraFront, cameraUp)) * delta;

		//update matricies
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::Resize(uint32_t width, uint32_t height)
	{
		m_AspectRation = static_cast<float>(width) / static_cast<float>(height);
		RecalculateViewMatrix();

	}

	void PerspectiveCamera::OnMouseEventHandler(Event & e)
	{
		if (e.GetEventType() == EventType::MouseMoved)
		{
			MouseMovedEvent& mouseEvent = (MouseMovedEvent&)e;
			float xpos = mouseEvent.GetX();
			float ypos = mouseEvent.GetY();

			if (firstMouse)
			{
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
				return;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			lastX = xpos;
			lastY = ypos;


			/*
					float deltaTime = Application::Get().GetElapsedTime();
			unsigned int width = Application::Get().GetWindow().GetWindowWidth();
			unsigned int height = Application::Get().GetWindow().GetWindowHeight();

			yaw += (ypos - (height / 2.0f))*deltaTime*0.1f;
			pitch += (xpos - (width / 2.0f))*deltaTime*0.1f;
			*/
			float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(direction);

			RecalculateViewMatrix();
		}
	}

	void PerspectiveCamera::ProcessEventWindowResize(Event & e)
	{
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& eventResize = (WindowResizeEvent&)e;

			m_AspectRation = (float)eventResize.GetWidth() / (float)eventResize.GetHeight();
			RecalculateViewMatrix();
		}
	}

	void PerspectiveCamera::OnMouseScrollEvent(Event & e)
	{
		if (e.GetEventType() == EventType::MouseScrolled)
		{
			MouseScrolledEvent& eventScroll = (MouseScrolledEvent&)e;
			float offset = eventScroll.GetOffsetY();
			LOG_INFO("scroll offset {0}", offset);
			m_FieldOfView += offset;
			if (m_FieldOfView > 120.0f)
				m_FieldOfView = 120.0f;

			if (m_FieldOfView < 20.0f)
				m_FieldOfView = 20.0f;

			RecalculateViewMatrix();
		}
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		float width = static_cast<float>(Application::Get()->GetWindow().GetWidth());
		float height = static_cast<float>(Application::Get()->GetWindow().GetHeight());

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + cameraFront, cameraUp);
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FieldOfView), m_AspectRation, m_Near, m_Far);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewMatrix = glm::mat4(1.0f);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
