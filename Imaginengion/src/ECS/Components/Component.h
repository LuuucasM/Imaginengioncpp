#pragma once

#include <typeindex>
namespace IM {
	struct Component {
		virtual ~Component() = default;
		std::type_index GetType() const { return typeid(*this); }
	};
}