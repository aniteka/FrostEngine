#pragma once

#include <memory>
#include <vector>
#include <array>

namespace core
{
	template<class T>
	using sptr_t = std::shared_ptr<T>;
	template<class T>
	using wptr_t = std::weak_ptr<T>;
	template<class T>
	using uptr_t = std::unique_ptr<T>;

	template<class T>
	using dynamic_array_t = std::vector<T>;
	template<class T, std::size_t Size>
	using array_t = std::array<T, Size>;
}

