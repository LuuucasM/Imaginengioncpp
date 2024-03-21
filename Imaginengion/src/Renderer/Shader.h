#pragma once

#include <glm/glm.hpp>

#include <string>

namespace IM {

	enum class ShaderDataType {
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		UInt,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:   return 4;
		case ShaderDataType::Float2:  return 4 * 2;
		case ShaderDataType::Float3:  return 4 * 3;
		case ShaderDataType::Float4:  return 4 * 4;
		case ShaderDataType::Mat3:    return 4 * 3 * 3;
		case ShaderDataType::Mat4:    return 4 * 4 * 4;
		case ShaderDataType::Int:     return 4;
		case ShaderDataType::UInt:     return 4;
		case ShaderDataType::Int2:    return 4 * 2;
		case ShaderDataType::Int3:    return 4 * 3;
		case ShaderDataType::Int4:    return 4 * 4;
		case ShaderDataType::Bool:    return 1;
		}
		IMAGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	static std::unordered_map<std::string, ShaderDataType> StrToDataType = {
		{"float", ShaderDataType::Float},
		{"vec2", ShaderDataType::Float2},
		{"vec3", ShaderDataType::Float3},
		{"vec4", ShaderDataType::Float4},
		{"mat3", ShaderDataType::Mat3},
		{"mat4", ShaderDataType::Mat4},
		{"int", ShaderDataType::Int},
		{"uint", ShaderDataType::UInt},
		//{"int", ShaderDataType::Int2},
		//{"int", ShaderDataType::Int3},
		//{"int", ShaderDataType::Int4},
		{"bool", ShaderDataType::Int}
	};

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool bNormalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), bNormalized(normalized) {

		}

		uint32_t GetComponentCount() const {
			switch (Type) {
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::UInt:    return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}
			IMAGINE_CORE_ASSERT(false, "Unkown ShaderDataType!");
			return 0;
		}
	};
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		const std::vector<BufferElement>& GetLayout() {
			return _BufferElements;
		}

		const uint32_t GetStride() {
			return _BufferStride;
		}

		virtual void SetValue(const std::string& name, bool value) = 0;
		virtual void SetValue(const std::string& name, int value) = 0;
		virtual void SetValue(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetValue(const std::string& name, float value) = 0;
		virtual void SetValue(const std::string& name, glm::vec2 value) = 0;
		virtual void SetValue(const std::string& name, glm::vec3 value) = 0;
		virtual void SetValue(const std::string& name, glm::vec4 value) = 0;
		virtual void SetValue(const std::string& name, glm::mat3 value) = 0;
		virtual void SetValue(const std::string& name, glm::mat4 value) = 0;

		virtual const std::string& GetName() const { return _Name; }

		static RefPtr<Shader> Create(const std::string& filepath);
	protected:
		uint32_t _ProgramID = 0;
		std::string _Filepath;
		std::string _Name;

		void CalculateOffsets() {
			uint32_t offset = 0;
			for (auto& element : _BufferElements) {
				element.Offset = offset;
				offset += element.Size;
			}
		}

		void CalculateStride() {
			_BufferStride = 0;
			for (auto& element : _BufferElements) {
				_BufferStride += element.Size;
			}
		}

		std::vector<BufferElement> _BufferElements;
		std::unordered_map<std::string, int> _Uniforms;
		uint32_t _BufferStride = 0;
	};
}