#include "impch.h"
#include "SceneSerializer.h"

#include "ECS/Entity.h"

#include "ECS/Components.h"

#include <yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace IM {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	namespace {
		std::string RigidBody2DBodyTypeToString(C_RigidBody2D::BodyType bodyType) {
			switch (bodyType) {
				case C_RigidBody2D::BodyType::Static: return "Static";
				case C_RigidBody2D::BodyType::Dynamic: return "Dynamic";
				case C_RigidBody2D::BodyType::Kinematic: return "Kinematic";
			}
			IMAGINE_CORE_ASSERT(0, "Unknown bodyType in RigidBody2DBodyTypeToString!");
			return {};
		}
		C_RigidBody2D::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString) {
			if (bodyTypeString == "Static") return C_RigidBody2D::BodyType::Static;
			if (bodyTypeString == "Dynamic") return C_RigidBody2D::BodyType::Dynamic;
			if (bodyTypeString == "Kinematic") return C_RigidBody2D::BodyType::Kinematic;
			IMAGINE_CORE_ASSERT(0, "Unknown bodyTypeString in RigidBody2DBodyTypeFromString!");
			return C_RigidBody2D::BodyType::Static;
		}
	}

	SceneSerializer::SceneSerializer(const RefPtr<Scene>& scene)
		: _Scene(scene){

	}

	static void SerializeEntity(YAML::Emitter& out, Entity& entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<C_Name>()) {
			out << YAML::Key << "Name Component";

			out << YAML::BeginMap;
			auto& name = entity.GetComponent<C_Name>().Name;
			out << YAML::Key << "Name" << YAML::Value << name;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<C_Transform>()) {
			out << YAML::Key << "Transform Component";
			
			out << YAML::BeginMap;
			auto& transform = entity.GetComponent<C_Transform>();
			out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<C_Camera>()) {
			out << YAML::Key << "Camera Component";

			out << YAML::BeginMap;
			auto& camera = entity.GetComponent<C_Camera>();
			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera._ProjectionType;
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera._PerspectiveFOV;
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera._PerspectiveicNear;
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera._PerspectiveFar;
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera._OrthographicSize;
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera._OrthographicNear;
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera._OrthographicFar;
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << camera._bPrimary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << camera._bFixedAspectRatio;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<C_SpriteRenderer>()) {
			out << YAML::Key << "Sprite Renderer Component";

			out << YAML::BeginMap;
			auto& spriteRenderer = entity.GetComponent<C_SpriteRenderer>();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.Color;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<C_CircleRenderer>()) {
			out << YAML::Key << "Circle Renderer Component";

			out << YAML::BeginMap;
			auto& circleRenderer = entity.GetComponent<C_CircleRenderer>();
			out << YAML::Key << "Color" << YAML::Value << circleRenderer.Color;
			out << YAML::Key << "Radius" << YAML::Value << circleRenderer.Radius;
			out << YAML::Key << "Thickness" << YAML::Value << circleRenderer.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRenderer.Fade;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<C_RigidBody2D>()) {
			out << YAML::Key << "Rigid Body 2D";

			out << YAML::BeginMap;
			auto& rigidBody = entity.GetComponent<C_RigidBody2D>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rigidBody._Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rigidBody._bFixedRotation;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<C_Collider2D>()) {
			out << YAML::Key << "Collider 2D";

			out << YAML::BeginMap;
			auto& collider = entity.GetComponent<C_Collider2D>();
			out << YAML::Key << "Offset" << YAML::Value << collider._Offset;
			out << YAML::Key << "Size" << YAML::Value << collider._Size;
			out << YAML::Key << "Density" << YAML::Value << collider._Density;
			out << YAML::Key << "Friction" << YAML::Value << collider._Friction;
			out << YAML::Key << "Restitution" << YAML::Value << collider._Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << collider._RestitutionThreshold;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeText(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << _Scene->_Name;
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		std::unordered_set<uint32_t>& entities = _Scene->_ECSManager.GetAllEntityID();
		for (auto ent : entities) {
			Entity entity = { ent, _Scene.get() };
			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

	}
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		//not implemented
		IMAGINE_CORE_ASSERT(false, "");
	}
	bool SceneSerializer::DeSerializeText(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"]) {
			return false;
		}
		std::string sceneName = data["Scene"].as<std::string>();
		_Scene->SetName(sceneName);
		IMAGINE_CORE_TRACE("Deserializing scene '{}'", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto nameComponent = entity["Name Component"];
				if (nameComponent) {
					name = nameComponent["Name"].as<std::string>();
				}

				IMAGINE_CORE_TRACE("Deserialized entity with ID = {}, name = {}", uuid, name);

				Entity e = _Scene->CreateEntityWithUUID(uuid, name);
				
				auto transformComponent = entity["Transform Component"];
				if (transformComponent) {
					auto& transform = e.GetComponent<C_Transform>();
					transform.Translation = transformComponent["Translation"].as<glm::vec3>();
					transform.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					transform.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["Camera Component"];
				if (cameraComponent) {
					auto& cc = e.AddComponent<C_Camera>();
					auto cameraProps = cameraComponent["Camera"];
					cc._ProjectionType = (IM::C_Camera::ProjectionType)cameraProps["ProjectionType"].as<int>();

					cc._PerspectiveFOV = cameraProps["PerspectiveFOV"].as<float>();
					cc._PerspectiveicNear = cameraProps["PerspectiveNear"].as<float>();
					cc._PerspectiveFar = cameraProps["PerspectiveFar"].as<float>();
					cc._OrthographicSize = cameraProps["OrthographicSize"].as<float>();
					cc._OrthographicNear = cameraProps["OrthographicNear"].as<float>();
					cc._OrthographicFar = cameraProps["OrthographicFar"].as<float>();

					cc._bPrimary = cameraComponent["Primary"].as<bool>();
					cc._bFixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["Sprite Renderer Component"];
				if (spriteRendererComponent) {
					auto& src = e.AddComponent<C_SpriteRenderer>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				auto circleRendererComponent = entity["Circle Renderer Component"];
				if (circleRendererComponent) {
					auto& src = e.AddComponent<C_CircleRenderer>();
					src.Color = circleRendererComponent["Color"].as<glm::vec4>();
					src.Radius = circleRendererComponent["Radius"].as<float>();
					src.Thickness = circleRendererComponent["Thickness"].as<float>();
					src.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto rigidBody = entity["Rigid Body 2D"];
				if (rigidBody) {
					auto& src = e.AddComponent<C_RigidBody2D>();
					src._Type = RigidBody2DBodyTypeFromString(rigidBody["BodyType"].as<std::string>());
					src._bFixedRotation = rigidBody["FixedRotation"].as<bool>();
				}

				auto collider = entity["Collider 2D"];
				if (collider) {
					auto& src = e.AddComponent<C_Collider2D>();
					src._Offset = collider["Offset"].as<glm::vec2>();
					src._Size = collider["Size"].as<glm::vec2>();
					src._Density = collider["Density"].as<float>();
					src._Friction = collider["Friction"].as<float>();
					src._Restitution = collider["Restitution"].as<float>();
					src._RestitutionThreshold = collider["RestitutionThreshold"].as<float>();
				}
			}
		}
		return true;
	}
	bool SceneSerializer::DeSerializeRuntime(const std::string& filepath)
	{
		//not implemented
		IMAGINE_CORE_ASSERT(false, "");
		return false;
	}
}