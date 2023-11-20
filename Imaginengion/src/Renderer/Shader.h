#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace IM {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string &fragmentSrc);
		virtual ~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform(const std::string& name, bool value);
		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, glm::vec2 value);
		void SetUniform(const std::string& name, glm::vec3 value);
		void SetUniform(const std::string& name, glm::vec4 value);
		void SetUniform(const std::string& name, glm::mat4 value);
		/*
		template <class T>
		void SetUniform(const std::string& name, T value) {}
		template<>
		void SetUniform<bool>(const std::string& name, bool value) {
			GLint location = glGetUniformLocation(_ProgramID, name.c_str());
			IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
			glUniform1i(location, (int)value);
		}
		template<>
		void SetUniform<int>(const std::string& name, int value) {
			GLint location = glGetUniformLocation(_ProgramID, name.c_str());
			IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
			glUniform1i(location, value);
		}
		template<>
		void SetUniform<float>(const std::string& name, float value) {
			GLint location = glGetUniformLocation(_ProgramID, name.c_str());
			IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
			glUniform1f(location, value);
		}
		template<>
		void SetUniform<glm::vec2>(const std::string& name, glm::vec2 value) {
			GLint location = glGetUniformLocation(_ProgramID, name.c_str());
			IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
			glUniform2f(location, value.x, value.y);
		}
		template<>
		void SetUniform<glm::vec3>(const std::string& name, glm::vec3 value) {
			GLint location = glGetUniformLocation(_ProgramID, name.c_str());
			IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
			glUniform3f(location, value.x, value.y, value.z);
		}
		template<>
		void SetUniform<glm::vec4>(const std::string& name, glm::vec4 value) {
			GLint location = glGetUniformLocation(_ProgramID, name.c_str());
			IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}
		template<>
		void SetUniform<glm::mat4>(const std::string& name, glm::mat4 value) {
			GLint location = glGetUniformLocation(_ProgramID, name.c_str());
			IMAGINE_CORE_ASSERT(location != -1, "Cannot find union with the given name");
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
		}
		*/
	private:
		uint32_t _ProgramID;
	};
}