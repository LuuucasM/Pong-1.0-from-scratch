#include "ComponentManager.h"

//put all the components after this comment
#include "Components.h"

void ComponentManager::InitComponents() {
	RegisterComponent<C_RigidBody2D>();
	RegisterComponent<C_Movement>();
	RegisterComponent<C_Collision>();
}