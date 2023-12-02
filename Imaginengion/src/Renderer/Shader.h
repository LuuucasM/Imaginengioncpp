#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace IM {
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static RefPtr<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	private:
		uint32_t _ProgramID = 0;
	};
}