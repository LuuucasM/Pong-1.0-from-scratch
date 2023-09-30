#include "CollisionSystem.h"
#include "ECSCoordinator.h"
#include "Components.h"
#include "Math.h"

extern ECSCoordinator ECSCoord;

void CollisionSystem::Init() {

}

void CollisionSystem::Update() {
	for (auto entity1 : Entities) {
		for (auto entity2 : Entities) {
			if (entity1 != entity2) {
				auto ent1Collision = ECSCoord.GetComponent<C_Collision>(entity1);
				auto ent2Collision = ECSCoord.GetComponent<C_Collision>(entity2);
				if (ent1Collision->CollisionFunctions.find(CantorFunction(ent1Collision->CollisionClass, ent2Collision->CollisionClass)) != ent1Collision->CollisionFunctions.end()) {
					auto ent1rigidBody = ECSCoord.GetComponent<C_RigidBody2D>(entity1);
					auto ent2rigidBody = ECSCoord.GetComponent<C_RigidBody2D>(entity2);
					if (ent1rigidBody->Position.x + ent1rigidBody->Size.x >= ent2rigidBody->Position.x
						&& ent2rigidBody->Position.x + ent2rigidBody->Size.x >= ent1rigidBody->Position.x) {
						if (ent1rigidBody->Position.y + ent1rigidBody->Size.y >= ent2rigidBody->Position.y
							&& ent2rigidBody->Position.y + ent2rigidBody->Size.y >= ent1rigidBody->Position.y) {
							ent1Collision->CollisionFunctions[CantorFunction(ent1Collision->CollisionClass, ent2Collision->CollisionClass)](entity1, entity2);
						}
					}
				}
			}
		}
	}
}