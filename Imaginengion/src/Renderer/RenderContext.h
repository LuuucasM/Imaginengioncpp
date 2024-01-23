#pragma once

namespace IM {
	class RenderContext
	{
	public:
		virtual ~RenderContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static ScopePtr<RenderContext> Create(void* window);
	};
}

