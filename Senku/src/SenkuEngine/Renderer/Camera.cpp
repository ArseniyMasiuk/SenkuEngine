#include "PrecompiledHeader.h"
#include "Camera.h"
#include "SenkuEngine\Core\Application.h"
#include "glm\ext\matrix_transform.hpp"
#include "glm\ext\matrix_clip_space.hpp"

#include "SenkuEngine\EventSystem\MouseEvent.h"
#include "SenkuEngine\Core\Input.h"

namespace Senku
{
	PerspectiveCamera::PerspectiveCamera(glm::vec3 position, float fieldOfView, float aspectRation, float zNear, float zFar)
		:m_FieldOfView(fieldOfView), m_AspectRation(aspectRation), m_Far(zFar), m_Near(zNear), m_Position(position)
	{
		cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		m_ViewMatrix = glm::mat4(1.0f);

		// calculate new position
		float distance = glm::distance(m_Position, origin);

		//LOG_INFO("Xoffset {}, Yoffset {}, distance {}", xoffset, yoffset, distance);
		// todo: camera initialization process sould be improved;

		glm::vec3 posRelatedOrigin = m_Position - origin;

		posRelatedOrigin.x = distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		posRelatedOrigin.y = distance * cos(glm::radians(yaw));
		posRelatedOrigin.z = distance * sin(glm::radians(yaw)) * sin(glm::radians(pitch));

		m_Position = posRelatedOrigin + origin;

		cameraFront = glm::normalize(origin - m_Position);
		RecalculateViewMatrix();
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::OnUpdate(float timeStep)
	{
		delta = cameraSpeed * timeStep;


		// calculate mouse delta
		float xpos = Input::GetMouseX();
		float ypos = Input::GetMouseY();

		if (firstMouse)
		{
			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();
			firstMouse = false;
			//return;
		}

		xoffset = xpos - lastX;
		yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		//RecalculateViewMatrix();
	}

	void PerspectiveCamera::Resize(uint32_t width, uint32_t height)
	{
		m_AspectRation = static_cast<float>(width) / static_cast<float>(height);
		RecalculateViewMatrix();

	}

	bool PerspectiveCamera::OnEditorCameraUpdate(Event & event)
	{
		if (event.isInCategory(EventCategory::EventCategoryMouse) && (event.GetEventType() == EventType::MouseMoved || event.GetEventType() == EventType::MouseScrolled))
		{

			if (Input::IsKeyPressed(Key::LeftControl) && Input::IsMouseButtonPressed(Mouse::ButtonLeft)) // move in horizont plane // this work a bit wrong when cameta is looking from top to bottom
			{
				//LOG_WARN("move by hosrizont");
				m_Position -= (yoffset * delta* 0.7f)* glm::vec3(cameraFront.x, 0, cameraFront.z);
				m_Position -= glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0, cameraFront.z), cameraUp)) * (xoffset * delta* 0.7f);

				origin -= (yoffset * delta* 0.7f)* glm::vec3(cameraFront.x, 0, cameraFront.z);
				origin -= glm::normalize(glm::cross(glm::vec3(cameraFront.x, 0, cameraFront.z), cameraUp)) * (xoffset * delta* 0.7f);


				RecalculateViewMatrix();

				return true;
			}
			else if (Input::IsKeyPressed(Key::LeftShift) && Input::IsMouseButtonPressed(Mouse::ButtonLeft)) // move up and down
			{
				//LOG_WARN("up and down");
				m_Position -= (yoffset * delta * 0.7f)* glm::vec3(0, 1, 0);
				m_Position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * (xoffset * delta * 0.7f);

				origin -= (yoffset * delta * 0.7f)* glm::vec3(0, 1, 0);
				origin -= glm::normalize(glm::cross(cameraFront, cameraUp)) * (xoffset * delta * 0.7f);

				//LOG_INFO("origin x{}, y{} z{}", origin.x, origin.y, origin.z);
				//LOG_INFO("position x{}, y{} z{}", m_Position.x, m_Position.y, m_Position.z);

				RecalculateViewMatrix();

				return true;
			}
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft)) // orbit movement
			{
				//LOG_WARN(" orbit movement");


				pitch += xoffset * delta;
				yaw += yoffset * delta;

				if (yaw > 179.9f)
					yaw = 179.9f;
				if (yaw < 0.001f)
					yaw = 0.001f;

				if (pitch > 360.0f)
					pitch -= 360.0f;
				if (pitch < 0.0f)
					pitch = 360.0f + pitch;

				// calculate new position
				float distance = glm::distance(m_Position, origin);

				//LOG_INFO("Xoffset {}, Yoffset {}, distance {}", xoffset, yoffset, distance);


				glm::vec3 posRelatedOrigin = m_Position - origin;

				posRelatedOrigin.x = distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				posRelatedOrigin.y = distance * cos(glm::radians(yaw));
				posRelatedOrigin.z = distance * sin(glm::radians(yaw)) * sin(glm::radians(pitch));

				m_Position = posRelatedOrigin + origin;

				cameraFront = glm::normalize(origin - m_Position);
				RecalculateViewMatrix();
				return true;

	
			}
			else if (event.GetEventType() == EventType::MouseScrolled)
			{
				MouseScrolledEvent& eventScroll = (MouseScrolledEvent&)event;
				float offset = eventScroll.GetOffsetY();

				float distance = glm::distance(m_Position, origin);

				distance -= offset * delta*50; // todo: distance should not be zero or negative
				//LOG_INFO("ofset {}  distance {}", offset, distance);
				glm::vec3 posRelatedOrigin = m_Position - origin;

				posRelatedOrigin.x = distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				posRelatedOrigin.y = distance * cos(glm::radians(yaw));
				posRelatedOrigin.z = distance * sin(glm::radians(yaw)) * sin(glm::radians(pitch));

				m_Position = posRelatedOrigin + origin;

				cameraFront = glm::normalize(origin - m_Position);
				RecalculateViewMatrix();


			}

		}

		return false;
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
