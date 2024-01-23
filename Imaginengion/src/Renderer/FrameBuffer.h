#pragma once

namespace IM {
	struct FrameBufferSpecification {
		size_t Width = 0, Height = 0;
		uint32_t Samples = 1;

		bool bSwapChainTarget = false;
	};
	class FrameBuffer
	{
	public:
		const FrameBufferSpecification& GetSpecification() const { return _Specification; };

		static RefPtr<FrameBuffer> Create(const FrameBufferSpecification& spec);

		uint32_t GetColorAttachmentID() const { return _ColorAttachment; };

		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(size_t width, size_t height) = 0;
	protected:
		FrameBufferSpecification _Specification;
		uint32_t _BufferID = 0;
		uint32_t _ColorAttachment = 0;
		uint32_t _DepthAttachment = 0;
	};
}
