#include "ComponentManager.h"

//put all the components after this comment
#include "Scene/Components.h"

void ComponentManager::InitComponents() {
	//register all the build in engine components
	RegisterComponent<C_Transform>();
	RegisterComponent<C_SpriteRenderer>();
}