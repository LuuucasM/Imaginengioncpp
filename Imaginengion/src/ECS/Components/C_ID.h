#pragma once

#include "Core/UUID.h"

namespace IM {
	struct C_ID {
		UUID _ID;
		C_ID() = default;
		C_ID(UUID id) 
			: _ID(id){}
		C_ID(const C_ID&) = default;
	};
}