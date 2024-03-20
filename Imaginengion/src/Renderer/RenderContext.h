#pragma once

namespace IM {
	class RenderContext
	{
	public:
		RenderContext(void* window);
		~RenderContext() = default;

		void Init();
		void SwapBuffers();

		static ScopePtr<RenderContext> Create(void* window) {
			return CreateScopePtr<RenderContext>(window);
		};
	};
}