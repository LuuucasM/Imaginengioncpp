#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace IM {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		~OpenGLTexture2D();

		inline uint32_t GetWdith() const override { return _Width; };
		inline uint32_t GetHeight() const override { return _Height; };

		void SetData(void* data, uint32_t size) override;

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;
	private:
		uint32_t _Width;
		uint32_t _Height;
		GLenum _InternalFormat, _DataFormat;
	};
}
