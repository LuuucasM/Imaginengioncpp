#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace IM {
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		inline uint32_t GetWdith() const override { return _Width; };
		inline uint32_t GetHeight() const override { return _Height; };
		void Bind(uint32_t slot = 0) const override;
	private:
		uint32_t _Width;
		uint32_t _Height;
	};
}
