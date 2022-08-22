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
		float m_Intensity = 1.0f; // its line in what maximum distance object gets 100% of the light
	};
}