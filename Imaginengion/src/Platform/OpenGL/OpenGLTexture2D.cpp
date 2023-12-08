#include "impch.h"
#include "OpenGLTexture2D.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace IM {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		IMAGINE_CORE_ASSERT(data, "Failed to load image in OpenGLTexture2D constructor!");

		_Width = width;
		_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		IMAGINE_CORE_ASSERT(internalFormat, "Texture format not supported in OpenGLTexture2D: {}", path);
		IMAGINE_CORE_ASSERT(dataFormat, "Texture format not supported in OpenGLTexture2D: {}", path);



		glCreateTextures(GL_TEXTURE_2D, 1, &_TextureID);
		glTextureStorage2D(_TextureID, 1, internalFormat, _Width, _Height);

		glTextureParameteri(_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_TextureID, 0, 0, 0, _Width, _Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &_TextureID);
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTexture(GL_TEXTURE_2D, _TextureID);
	}
}