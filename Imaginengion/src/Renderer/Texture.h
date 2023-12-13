#pragma once

#include "Core/Core.h"

#include <string>

namespace IM {
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWdith() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;
	protected:
		uint32_t _TextureID;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;
		static RefPtr<Texture2D> Create(uint32_t width, uint32_t height);
		static RefPtr<Texture2D> Create(const std::string& path);
	};
}