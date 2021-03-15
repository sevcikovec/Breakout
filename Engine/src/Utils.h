#pragma once
#include <memory>

namespace Engine {

	///
	/// Courtesy of https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw
	///
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

