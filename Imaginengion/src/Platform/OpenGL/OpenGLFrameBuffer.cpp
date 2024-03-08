#include "impch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace IM {

	static const uint32_t _MaxFrameBufferSize = 8192;

	static GLenum TextureTarget(bool multisampled) {
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
		glCreateTextures(TextureTarget(multisampled), count, outID);
	}

	static void BindTexture(bool multisampled, uint32_t id) {
		glBindTexture(TextureTarget(multisampled), id);
	}

	static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
		bool multisampled = samples > 1;
		if (multisampled){
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
		bool multisampled = samples > 1;
		if (multisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else {
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	static bool IsDepthFormat(FrameBufferTextureFormat format) {
		switch (format) {
			case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
		}
		return false;
	}

	static GLenum ImagineFBTextureFormatToGl(FrameBufferTextureFormat format) {
		switch (format) {
			case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
			case FrameBufferTextureFormat::RGBA16F: return GL_RGBA16F;
			case FrameBufferTextureFormat::RGBA32F: return GL_RGBA32F;
			case FrameBufferTextureFormat::RG32F: return GL_RG32F;
			case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			case FrameBufferTextureFormat::DEPTH32F: return GL_DEPTH32F_STENCIL8;
			case FrameBufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
		}
		IMAGINE_CORE_ASSERT(0, "Unkown FrameBufferTextureFormat in ImagineFBTextureFormatToGl!");
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
	{
		_Specification = spec;

		for (auto specification : _Specification._Attachments._Attachments) {
			if (!IsDepthFormat(specification._TextureFormat)) {
				_ColorAttachmentSpecs.emplace_back(specification);
			}
			else {
				_DepthAttachmentSpecs = specification;
			}
		}
		Invalidate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &_BufferID);
		glDeleteTextures(_ColorAttachments.size(), _ColorAttachments.data());
		glDeleteTextures(1, &_DepthAttachment);
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		if (_BufferID) {
			glDeleteFramebuffers(1, &_BufferID);
			glDeleteTextures(_ColorAttachments.size(), _ColorAttachments.data());
			glDeleteTextures(1, &_DepthAttachment);
			_ColorAttachments.clear();
			_DepthAttachment = 0;
		}
		glCreateFramebuffers(1, &_BufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, _BufferID);

		bool multisample = _Specification.Samples > 1;

		//Color Attachments
		if (_ColorAttachmentSpecs.size()) {
			_ColorAttachments.resize(_ColorAttachmentSpecs.size());
			CreateTextures(multisample, _ColorAttachments.data(), _ColorAttachments.size());
			for (size_t i = 0; i < _ColorAttachments.size(); ++i) {
				BindTexture(multisample, _ColorAttachments[i]);
				switch (_ColorAttachmentSpecs[i]._TextureFormat) {
					case FrameBufferTextureFormat::RGBA8:
						AttachColorTexture(_ColorAttachments[i], _Specification.Samples, GL_RGBA8, GL_RGBA, _Specification.Width, _Specification.Height, i);
						break;
					case FrameBufferTextureFormat::RED_INTEGER:
						AttachColorTexture(_ColorAttachments[i], _Specification.Samples, GL_R32UI, GL_RED_INTEGER, _Specification.Width, _Specification.Height, i);
						break;
				}
			}
		}
		//Depth attachment
		if (_DepthAttachmentSpecs._TextureFormat != FrameBufferTextureFormat::None) {
			CreateTextures(multisample, &_DepthAttachment, 1);
			BindTexture(multisample, _DepthAttachment);
			switch (_DepthAttachmentSpecs._TextureFormat) {
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				AttachDepthTexture(_DepthAttachment, _Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, _Specification.Width, _Specification.Height);
				break;
			}
		}

		if (_ColorAttachments.size() > 1) {
			IMAGINE_CORE_ASSERT(_ColorAttachments.size() <= 4, "OpenGLFrameBuffer does not support more than 4 color attachments at the moment!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(_ColorAttachments.size(), buffers);
		}
		else if (_ColorAttachments.empty()){
			glDrawBuffer(GL_NONE);
		}

		IMAGINE_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is incorrect in OpenGLFrameBuffer::Invalidate");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _BufferID);
		glViewport(0, 0, (GLsizei)_Specification.Width, (GLsizei)_Specification.Height);

		//ClearColorAttachment(1, 


	}
	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Resize(size_t width, size_t height)
	{
		if (width < 1 || height < 1 || width > _MaxFrameBufferSize || height > _MaxFrameBufferSize) {
			IMAGINE_CORE_WARN("Attempted to resize framebuffer to {}, {}", width, height);
			return;
		}
		_Specification.Width = width;
		_Specification.Height = height;

		Invalidate();
	}
	uint32_t OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		IMAGINE_CORE_ASSERT(attachmentIndex < _ColorAttachments.size(), "attachment index must be within bounds of color attachments.size() in ReadPixel()");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		uint32_t pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixelData);
		return pixelData;
	}
	void OpenGLFrameBuffer::ClearColorAttachment(uint32_t attachmentIndex, uint32_t value)
	{
		IMAGINE_CORE_ASSERT(attachmentIndex < _ColorAttachments.size(), "attachment index must be within bounds of color attachments.size() in ClearColorAttachment()");

		auto& spec = _ColorAttachmentSpecs[attachmentIndex];

		glClearTexImage(_ColorAttachments[attachmentIndex], 0,
			ImagineFBTextureFormatToGl(spec._TextureFormat), GL_UNSIGNED_INT, &value);
	}
}