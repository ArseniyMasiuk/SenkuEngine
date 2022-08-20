#pragma once

#include "glm\glm.hpp"

namespace Senku
{
	class DirectLight
	{
	public:
		glm::vec3 m_LightColor{ 1.0f };
		glm::vec3 m_Direction;
		float m_Distance = 500.0f;
		float m_Intensity = 1.0f;
		//glm::vec3 m_Ambient;
		//glm::vec3 m_Diffuse;
		//glm::vec3 m_Specular;
		// todo: add light intensity or so
	};
}