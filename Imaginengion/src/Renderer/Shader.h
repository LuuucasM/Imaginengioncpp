#pragma once

#include <string>

namespace IM {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string &fragmentSrc);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t _RendererID;
	};
}