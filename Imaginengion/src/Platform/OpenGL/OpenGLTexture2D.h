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

		uint32_t GetWidth() const override { return _Width; };
		uint32_t GetHeight() const override { return _Height; };
		uint32_t GetID() const override { return _TextureID; };

		void SetData(void* data, uint32_t size) override;

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		bool operator==(const Texture& other) const override { 
			return _TextureID == ((OpenGLTexture2D&)other)._TextureID; 
		};
	private:
		uint32_t _Width;
		uint32_t _Height;
		GLenum _InternalFormat, _DataFormat;
	};
}
