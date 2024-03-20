#pragma once

namespace IM {
	class IndexBuffer {
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		//virtual void SetData() = 0;

		void Bind() const;
		void Unbind() const;

		static RefPtr<IndexBuffer> Create(uint32_t* indices, uint32_t count) {
			return CreateRefPtr<IndexBuffer>(indices, count);
		};

		uint32_t GetCount() const { return _Count; }
	protected:
		uint32_t _Count;
		uint32_t _BufferID;
	};
}