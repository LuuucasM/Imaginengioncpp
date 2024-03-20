#pragma once

#include "Core/Core.h"

namespace IM {
	class UniformBuffer {
	public:

		UniformBuffer(uint32_t size, uint32_t binding);
		~UniformBuffer();

		void SetData(const void* data, uint32_t size, uint32_t offset = 0);

		static RefPtr<UniformBuffer> Create(uint32_t size, uint32_t binding) {
			return CreateRefPtr<UniformBuffer>(size, binding);
		};
	private:
		uint32_t _BufferID;
	};
}