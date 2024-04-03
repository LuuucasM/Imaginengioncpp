#pragma once

#include <xhash>

namespace IM {
	class UUID {
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return _UUID; }
	private:
		uint64_t _UUID;
	};
}

namespace std {
	template<>
	struct hash<IM::UUID> {
		std::size_t operator()(const IM::UUID& uuid) const {
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}