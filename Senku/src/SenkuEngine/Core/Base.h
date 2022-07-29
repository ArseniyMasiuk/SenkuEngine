#pragma once
#include "PlatformDetection.h"

#include "Assert.h"


#include <memory>

namespace Senku
{

	// Hazel engine by Cherno, i stole it from there )) a bit later will investigate all this big brain move, for now i dont understand all from this line

// todo: make sure that subscribed object still exist
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define BIT(x) (1<<x)


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


#include "SenkuEngine\EventSystem\KeyEvent.h"
#include "SenkuEngine\EventSystem\MouseEvent.h"
#include "SenkuEngine\Core\Input.h"
#include "SenkuEngine\EventSystem\ApplicationEvent.h"
