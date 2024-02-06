#include "impch.h"
#include "Scene.h"

#include "ECS/Components.h"
#include "Renderer/Renderer.h"

#include <glm/glm.hpp>

#include "ECS/Entity.h"

namespace IM {
	Scene::Scene()
	{
		//uint32_t entity = _ECSManager.CreateEntity();
		//C_Transform& transformComponent = _ECSManager.AddComponent<C_Transform>(entity);
		//C_SpriteRenderer& spriteRendererComponent = _ECSManager.AddComponent<C_SpriteRenderer>(entity);
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
	void Scene::OnUpdate(float dt)
	{

		//update scripts on update function
		auto group = _ECSManager.GetGroup<C_NativeScript>();
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
		auto entities = _ECSManager.GetGroup<C_Transform, C_Camera>();
		for (auto ent : entities) {
			auto ent_cam = _ECSManager.GetComponent<C_Camera>(ent);
			if (ent_cam._bPrimary) {
				mainCamera = &ent_cam;
				cameraTransform = &_ECSManager.GetComponent<C_Transform>(ent);
				break;
			}
		}

		if (mainCamera) {

			//this is the main render system for rendering in 2d
			Renderer::R2D::BeginScene(*mainCamera, *cameraTransform);//---------------------------------

			auto group = _ECSManager.GetGroup<C_Transform, C_SpriteRenderer>();
			for (auto entity : group) {
				auto [transform, sprite] = _ECSManager.GetComponents<C_Transform, C_SpriteRenderer>(entity);
				Renderer::R2D::DrawRect(transform.GetTransform(), sprite.Color);
			}

			Renderer::R2D::EndScene();//------------------------
		}
	}
	void Scene::OnViewportResize(size_t viewportWidth, size_t viewportHeight)
	{
		_ViewportWidth = viewportWidth;
		_ViewportHeight = viewportHeight;

		//this is a changing aspect ratio on cameras depending on viewport resizeing
		auto entities = _ECSManager.GetGroup<C_Camera>();
		for (auto ent : entities) {
			//auto& cam = _ECSManager.GetComponent<C_Camera>(ent);
			auto [transform, cam] = _ECSManager.GetComponents<C_Transform, C_Camera>(ent);
			if (!cam._bFixedAspectRatio) {
				cam.SetViewportSize(viewportWidth, viewportHeight);
			}
		}
	}
}