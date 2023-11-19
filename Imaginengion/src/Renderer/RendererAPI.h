#pragma once

namespace IM {
	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};
		inline static RendererAPI::API GetCurrentAPI() { return _RendererAPI; }
	private:
		static RendererAPI::API _RendererAPI;
	};
}