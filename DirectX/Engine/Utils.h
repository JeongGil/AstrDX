#pragma once
#include <memory>

template <typename T>
bool IsSameTarget(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b)
{
	return !a.owner_before(b) && !b.owner_before(a);
}

