#pragma once

#include <string>

namespace IM {
	struct C_Name {
		std::string Name;
		C_Name() = default;
		C_Name(const C_Name&) = default;
		C_Name(const std::string& name)
			: Name(name) {}
	};
}