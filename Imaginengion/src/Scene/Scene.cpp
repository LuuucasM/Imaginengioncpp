#include "impch.h"
#include "Scene.h"

#include "ECS/Components.h"
#include "ECS/Systems.h"
#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

#include "ECS/Entity.h"

namespace IM {
	Scene::Scene()
	{
		_ECSManager.RegisterComponent<C_Transform>();
		_ECSManager.RegisterComponent<C_SpriteRenderer>();
	}
	Scene::~Scene()
	{
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity e = { _ECSManager.CreateEntity(), this };
		e.AddComponent<C_Transform>();
		e.AddComponent<C_Name>(name);
		return e;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		_ECSManager.DestroyEntity(entity);
	}
	void Scene::OnUpdateRuntime(float dt)
	{
		_FPS = 1.0f / dt;
		//update scripts on update function
		auto& group = _ECSManager.GetGroup<C_NativeScript>();
		for (auto entity : group) {
			auto& script = _ECSManager.GetComponent<C_NativeScript>(entity);
			if (!script.Instance) {
				script.Instance = script.CreateScript();
				script.Instance->_Entity = { entity, this };
				script.Instance->OnCreate();
			}
			script.Instance->OnUpdate(dt);
		}


		//CHECKING TO SEE IF WE HAVE DEFINED A PRIMARY CAMERA
		//need to change this so that scene just directly holds a pointer to the primary camera instead of looping through cameras every time
		C_Camera *mainCamera = nullptr;
		C_Transform* cameraTransform = nullptr;
		auto& entities = _ECSManager.GetGroup<C_Transform, C_Camera>();
		for (auto ent : entities) {
			auto ent_cam = _ECSManager.GetComponent<C_Camera>(ent);
			if (ent_cam._bPrimary) {
				mainCamera = &ent_cam;
				cameraTransform = &_ECSManager.GetComponent<C_Transform>(ent);
				break;
			}
		}

		if (mainCamera) {
			Renderer::R2D::BeginScene(*mainCamera, *cameraTransform);
			auto& group = _ECSManager.GetGroup<C_Transform, C_SpriteRenderer>();
			for (auto entity : group) {
				auto [transform, sprite] = _ECSManager.GetComponents<C_Transform, C_SpriteRenderer>(entity);
				Renderer::R2D::DrawSprite(transform.GetTransform(), sprite, entity);
			}
			Renderer::R2D::EndScene();
		}
	}
	void Scene::OnUpdateEditor(float dt, EditorCamera& camera)
	{
		_FPS = 1.0f / dt;
		Renderer::R2D::BeginScene(camera);
		auto& group = _ECSManager.GetGroup<C_Transform, C_SpriteRenderer>();
		for (auto entity : group) {
			auto [transform, sprite] = _ECSManager.GetComponents<C_Transform, C_SpriteRenderer>(entity);
			Renderer::R2D::DrawSprite(transform.GetTransform(), sprite, entity);
		}
		Renderer::R2D::EndScene();
	}
	void Scene::OnViewportResize(size_t viewportWidth, size_t viewportHeight)
	{
		_ViewportWidth = viewportWidth;
		_ViewportHeight = viewportHeight;

		//this is a changing aspect ratio on cameras depending on viewport resizeing
		auto& entities = _ECSManager.GetGroup<C_Camera>();
		for (auto ent : entities) {
			//auto& cam = _ECSManager.GetComponent<C_Camera>(ent);
			auto [transform, cam] = _ECSManager.GetComponents<C_Transform, C_Camera>(ent);
			if (!cam._bFixedAspectRatio) {
				cam.SetViewportSize(viewportWidth, viewportHeight);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto& group = _ECSManager.GetGroup<C_Camera>();
		for (auto entity : group) {
			auto& cameraComponent = _ECSManager.GetComponent<C_Camera>(entity);
			if (cameraComponent._bPrimary) {
				return Entity(entity, this);
			}
		}
		return { 0, nullptr };
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		IMAGINE_CORE_ASSERT(0, "this should never happen! in OnComponentAdded")
	}

	template<>
	void Scene::OnComponentAdded<C_Transform>(Entity entity, C_Transform& component) {
		
	}
	template<>
	void Scene::OnComponentAdded<C_SpriteRenderer>(Entity entity, C_SpriteRenderer& component) {

	}
	template<>
	void Scene::OnComponentAdded<C_Name>(Entity entity, C_Name& component) {

	}
	template<>
	void Scene::OnComponentAdded<C_Camera>(Entity entity, C_Camera& component) {
		if (_ViewportWidth > 0 && _ViewportHeight > 0) {
			component.SetViewportSize(_ViewportWidth, _ViewportHeight);
		}
	}
	template<>
	void Scene::OnComponentAdded<C_NativeScript>(Entity entity, C_NativeScript& component) {

	}
}