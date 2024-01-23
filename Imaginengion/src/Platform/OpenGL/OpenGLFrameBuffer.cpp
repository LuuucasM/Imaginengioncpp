#include "impch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace IM {

	static const uint32_t _MaxFrameBufferSize = 8192;
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
	{
		_Specification = spec;
		Invalidate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &_BufferID);
		glDeleteTextures(1, &_ColorAttachment);
		glDeleteTextures(1, &_DepthAttachment);
	}
	void OpenGLFrameBuffer::Invalidate()
	{
		if (_BufferID) {
			glDeleteFramebuffers(1, &_BufferID);
			glDeleteTextures(1, &_ColorAttachment);
			glDeleteTextures(1, &_DepthAttachment);
		}
		glCreateFramebuffers(1, &_BufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, _BufferID);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, _ColorAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Specification.Width, _Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, _DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _Specification.Width, _Specification.Height);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _Specification.Width, _Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _DepthAttachment, 0);

		IMAGINE_CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is incorrect in OpenGLFrameBuffer::Invalidate");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _BufferID);
		glViewport(0, 0, _Specification.Width, _Specification.Height);
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
}