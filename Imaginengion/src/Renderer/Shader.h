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

		virtual void SetValue(const std::string& name, bool value) = 0;
		virtual void SetValue(const std::string& name, int value) = 0;
		virtual void SetValue(const std::string& name, float value) = 0;
		virtual void SetValue(const std::string& name, glm::vec2 value) = 0;
		virtual void SetValue(const std::string& name, glm::vec3 value) = 0;
		virtual void SetValue(const std::string& name, glm::vec4 value) = 0;
		virtual void SetValue(const std::string& name, glm::mat3 value) = 0;
		virtual void SetValue(const std::string& name, glm::mat4 value) = 0;

		virtual const std::string& GetName() const { return _Name; }

		static RefPtr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static RefPtr<Shader> Create(const std::string& filepath);
	protected:
		uint32_t _ProgramID = 0;
		std::string _Name;
	};
}