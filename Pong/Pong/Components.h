#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "ECSConstants.h"

#include <map>

enum CollisionClasses {
	PLAYER,
	TOPWALL,
	BOTTOMWALL,
	LEFTWALL,
	RIGHTWALL,
	BALL
};

struct C_RigidBody2D {
	glm::vec2 Position;
	float Rotation;
	glm::vec2 Size;
	bool toRender;
	std::string ShaderName;
};
struct C_Movement {
	glm::vec2 velocity;
	float speed;
	float direction;
	int whatever;
};

struct C_Collision {
	CollisionClasses CollisionClass;
	std::map<uint64_t, CollisionFunc> CollisionFunctions;
};