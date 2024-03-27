#pragma once

#include "Core/Core.h"

#include <string>

namespace IM {
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(uint32_t width, uint32_t height);
		Texture2D(const std::string& path);
		~Texture2D();

		uint32_t GetWidth() const override {
			return _Width;
		}
		uint32_t GetHeight() const override {
			return _Height;
		}
		uint32_t GetID() const override {
			return _TextureID;
		}

		void SetData(void* data, uint32_t size) override;

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;

		bool operator==(const Texture2D& other) const {
			return _TextureID == other._TextureID;
		}

		static RefPtr<Texture2D> Create(uint32_t width, uint32_t height) {
			return CreateRefPtr<Texture2D>(width, height);
		}
		static RefPtr<Texture2D> Create(const std::string& path) {
			return CreateRefPtr<Texture2D>(path);
		}
	private:
		uint32_t _Width;
		uint32_t _Height;
		uint32_t _TextureID;
	};
}