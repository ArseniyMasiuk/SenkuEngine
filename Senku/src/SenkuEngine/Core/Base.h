#pragma once
#include "PlatformDetection.h"

#include "Assert.h"
#include "SenkuEngine\EventSystem\ApplicationEvent.h"
#include "SenkuEngine\EventSystem\KeyEvent.h"
#include "SenkuEngine\EventSystem\MouseEvent.h"
#include "SenkuEngine\Core\Input.h"

#include <memory>

namespace Senku
{
	// not my code stole it from hazel 
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}