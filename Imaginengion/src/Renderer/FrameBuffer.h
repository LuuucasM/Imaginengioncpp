#pragma once



namespace IM {

	enum class FrameBufferTextureFormat {
		None = 0,

		//color
		RGBA8 = 1,
		RGBA16F = 2,
		RGBA32F = 3,
		RG32F = 4,
		RED_INTEGER = 5,

		//depth / stencil
		DEPTH32F = 6,
		DEPTH24STENCIL8 = 7,

		//defaults
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification {
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			: _TextureFormat(format) {}
		~FrameBufferTextureSpecification() = default;

		FrameBufferTextureFormat _TextureFormat = FrameBufferTextureFormat::None;
		//TODO: filter/wrap
	};

	struct FrameBufferAttachmentSpecification {
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(const std::initializer_list<FrameBufferTextureSpecification> attachments)
			: _Attachments(attachments) {}
		std::vector<FrameBufferTextureSpecification> _Attachments;
	};

	struct FrameBufferSpecification {
		size_t Width = 0, Height = 0;
		FrameBufferAttachmentSpecification _Attachments;
		uint32_t Samples = 1;

		bool bSwapChainTarget = false;
	};
	class FrameBuffer
	{
	public:
		const FrameBufferSpecification& GetSpecification() const { return _Specification; };

		static RefPtr<FrameBuffer> Create(const FrameBufferSpecification& spec) {
			return CreateRefPtr<FrameBuffer>(spec);
		};

		uint32_t GetColorAttachmentID(uint32_t index = 0) const { 
			IMAGINE_CORE_ASSERT(index < _ColorAttachments.size(), "Index for Color Attachment ID does not exist!"); 
			return _ColorAttachments[index]; 
		}

		FrameBuffer(const FrameBufferSpecification& spec);
		~FrameBuffer();

		void Invalidate();

		void Bind();
		void Unbind();

		void Resize(size_t width, size_t height);
		uint32_t ReadPixel(uint32_t attachmentIndex, int x, int y);

		void ClearColorAttachment(uint32_t attachmentIndex, uint32_t value);

	protected:
		std::vector<FrameBufferTextureSpecification> _ColorAttachmentSpecs;
		FrameBufferTextureSpecification _DepthAttachmentSpecs;
		FrameBufferSpecification _Specification;
		uint32_t _BufferID = 0;

		std::vector<uint32_t> _ColorAttachments;
		uint32_t _DepthAttachment;
	};
}
