#pragma once

#include "glm\glm.hpp"

namespace Senku
{
	class DirectLight
	{
	public:
		glm::vec3 m_LightColor{ 1.0f };
		glm::vec3 m_Direction;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;
		glm::vec3 m_Specular;
	};
}