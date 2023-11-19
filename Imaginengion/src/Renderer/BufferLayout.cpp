#include "impch.h"
#include "BufferLayout.h"

namespace IM {
	void BufferLayout::CalculateOffsets() {
		uint32_t offset = 0;
		for (auto& element : _Elements) {
			element.Offset = offset;
			offset += element.Size;
		}
	}
	void BufferLayout::CalculateStride() {
		_Stride = 0;
		for (auto& element : _Elements) {
			_Stride += element.Size;
		}
	}
}