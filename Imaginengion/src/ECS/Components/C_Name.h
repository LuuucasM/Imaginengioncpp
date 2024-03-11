#pragma once
#include "Component.h"

#include <string>

namespace IM {
	struct C_Name : Component {
		std::string Name;
		C_Name() = default;
		C_Name(const C_Name&) = default;
		C_Name(const std::string& name)
			: Name(name) {}
	};
}