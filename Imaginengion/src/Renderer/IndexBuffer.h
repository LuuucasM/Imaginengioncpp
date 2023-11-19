#pragma once

namespace IM {
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() = default;

		//virtual void SetData() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);

		inline uint32_t GetCount() const { return _Count; }
	protected:
		uint32_t _Count;
		uint32_t _BufferID;
	};
}