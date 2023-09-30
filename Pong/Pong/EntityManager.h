#pragma once

#include "ECSConstants.h"

#include <queue>
#include <array>

class EntityManager {

private:
	std::queue<EntityID> AvailableEntities{};
	std::array<ComponentMask, MAX_ENTITIES> ComponentMasks{};
	uint64_t NumLivingEntities = 0;
public:
	EntityManager() {
		for (EntityID i = 0; i < MAX_ENTITIES; i++) {
			AvailableEntities.push(i);
		}
	};
	~EntityManager() {

	};
	EntityID CreateEntity() {
		assert(NumLivingEntities < MAX_ENTITIES && "Hit entity limit");
		EntityID id = AvailableEntities.front();
		AvailableEntities.pop();
		NumLivingEntities++;

		return id;
	};
	void DestroyEntity(EntityID entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range");
		ComponentMasks[entity].reset();
		AvailableEntities.push(entity);
		--NumLivingEntities;
	};
	void AddComponent(EntityID entity, ComponentType component) {
		assert(entity < MAX_ENTITIES && "Entity out of range");
		assert(component < MAX_COMPONENTS && "Component type out of range");

		ComponentMasks[entity].set(component);
	};
	void RemoveComponent(EntityID entity, ComponentType component) {
		assert(entity < MAX_ENTITIES && "Entity out of range");
		assert(component < MAX_COMPONENTS && "Component type out of range");

		ComponentMasks[entity].reset(component);
	};
	ComponentMask GetComponentMask(EntityID entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range");
		return ComponentMasks[entity];
	};
};