#pragma once

#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

class ECSCoordinator {

private:
	std::unique_ptr<ComponentManager> CompManager;
	std::unique_ptr<EntityManager> EntManager;
	std::unique_ptr<SystemManager> SysManager;

public:
	ECSCoordinator() {};
	~ECSCoordinator() {};
	void Init() {
		CompManager = std::make_unique<ComponentManager>();
		EntManager = std::make_unique<EntityManager>();
		SysManager = std::make_unique<SystemManager>();

		CompManager->InitComponents();
		SysManager->InitSystems();
	}
	//entity stuff
	EntityID CreateEntity() {
		return EntManager->CreateEntity();
	}
	void DestroyEntity(EntityID entity) {
		EntManager->DestroyEntity(entity);
		CompManager->EntityDestroyed(entity);
		SysManager->RemoveEntity(entity);
	}
	//component stuff
	/*
	template<typename T>
	void RegisterComponent() {
		CompManager->RegisterComponent<T>();
	}*/
	template<typename T>
	void AddComponent(EntityID entity, T component) {
		EntManager->AddComponent(entity, CompManager->GetComponentType<T>());

		CompManager->AddComponent(entity, component);

		SysManager->EntityMaskChanged(entity, EntManager->GetComponentMask(entity));
	}
	template<typename T>
	void RemoveComponent(EntityID entity) {
		CompManager->RemoveComponent<T>(entity);

		EntManager->RemoveComponent(entity, CompManager->GetComponentType<T>());

		SysManager->EntityMaskChanged(entity, EntManager->GetComponentMask());
	}
	template<typename T>
	T *GetComponent(EntityID entity) {
		return CompManager->GetComponent<T>(entity);
	}
	template<typename T>
	ComponentType GetComponentType() {
		return CompManager->GetComponentType<T>();
	}
	//system stuff
	/*
	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		return SysManager->RegisterSystem<T>();
	}
	template<typename T>
	void SetSystemSignature(ComponentMask mask) {
		SysManager->SetMask<T>(mask);
	}*/
	template<typename T>
	std::shared_ptr<System> GetSystem() {
		return SysManager->GetSystem<T>();
	}
};