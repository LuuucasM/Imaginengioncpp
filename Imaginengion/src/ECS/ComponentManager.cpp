#include "impch.h"
#include "ComponentManager.h"

//put all the components after this comment
#include "Components.h"

void ComponentManager::InitComponents() {
	//register all the build in engine components
	RegisterComponent<IM::C_Transform>();
	RegisterComponent<IM::C_SpriteRenderer>();
}