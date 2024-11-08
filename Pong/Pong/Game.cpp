#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"
#include "ResourceManager.h"
#include "ECSCoordinator.h"

#include "Components.h"
#include "Math.h"

#include <time.h>

extern ECSCoordinator ECSCoord;
extern Game Pong;

void BallToPaddle(EntityID ball, EntityID paddle) {
	auto ballMovement = ECSCoord.GetComponent<C_Movement>(ball);
	auto paddleMovement = ECSCoord.GetComponent<C_Movement>(paddle);
	ballMovement->velocity.x *= -1.0f;
	ballMovement->velocity.x += 1.0f;
	ballMovement->velocity.y += 0.10f * (paddleMovement->speed * paddleMovement->direction);
}
void BallToTopWall(EntityID ball, EntityID topwall) {
	auto ballMovement = ECSCoord.GetComponent<C_Movement>(ball);
	ballMovement->velocity.y *= -1.0f;
}
void BallToBottomWall(EntityID ball, EntityID bottomwall) {
	auto ballMovement = ECSCoord.GetComponent<C_Movement>(ball);
	ballMovement->velocity.y *= -1.0f;
}
void BallToLeftWall(EntityID ball, EntityID leftwall) {
	Pong.Close();
}
void BallToRightWall(EntityID ball, EntityID rightwall) {
	Pong.Close();
}

Game::Game(unsigned int width, unsigned int height) {
	State = GameState::GAMEPLAY_STATE;
	Width = width;
	Height = height;
}
Game::~Game() {

}
void Game::Init(GLFWwindow* window) {
	Window = window;
	auto shader = ResourceManager::LoadShader("shader.vs", "shader.fs", nullptr, "paddle");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	shader.use();
	shader.setUniform<int>("image", 0);
	shader.setUniform<glm::mat4>("projection", projection);
	shader.setUniform<glm::vec3>("spriteColor", glm::vec3(1.0f, 1.0f, 1.0f));

	ECSCoord.Init();

	//player character
	PlayerCharacter = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody2D>(PlayerCharacter, { glm::vec2(0.0f, 250.0f), 0.0f, glm::vec2(15.0f, 100.0f), true, "paddle" });
	ECSCoord.AddComponent<C_Movement>(PlayerCharacter, { glm::vec2(0.0f, 0.0f), 7.0f, 0.0f });
	ECSCoord.AddComponent<C_Collision>(PlayerCharacter, {CollisionClasses::PLAYER});

	//top wall
	TopWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody2D>(TopWall, { glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2((float)Width, 2.0f), false, "" });
	ECSCoord.AddComponent<C_Collision>(TopWall, {CollisionClasses::TOPWALL});

	//bottom wall
	BottomWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody2D>(BottomWall, { glm::vec2(0.0f, (float)Height-1.0f), 0.0f, glm::vec2((float)Width, 2.0f), false, "" });
	ECSCoord.AddComponent<C_Collision>(BottomWall, {CollisionClasses::BOTTOMWALL});

	//leftwall
	LeftWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody2D>(LeftWall, { glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(2.0f, (float)Height), false, ""});
	ECSCoord.AddComponent<C_Collision>(LeftWall, {CollisionClasses::LEFTWALL});

	//right wall
	RightWall = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody2D>(RightWall, { glm::vec2((float)Width-1.0f, 0.0f), 0.0f, glm::vec2(2.0f, (float)Height), false, "" });
	ECSCoord.AddComponent<C_Collision>(RightWall, {CollisionClasses::RIGHTWALL});

	//ball
	Ball = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody2D>(Ball, { glm::vec2((float)Width/2, (float)Height/2), 0.0f, glm::vec2(11.0f, 11.0f), true, "paddle"});
	srand((unsigned int)time(0));
	float xvel = 0;
	float yvel = (float)(rand() % 2)+2.0f;
	if (rand() % 2 == 1) {
		xvel = (rand() % 3) + 3.0f;
	}
	else {
		xvel = -(rand() % 3) - 3.0f;
	}
	ECSCoord.AddComponent<C_Movement>(Ball, { glm::vec2(xvel, yvel) });
	ECSCoord.AddComponent<C_Collision>(Ball, {CollisionClasses::BALL});

	auto ballCollision = ECSCoord.GetComponent<C_Collision>(Ball);
	ballCollision->CollisionFunctions.insert({ CantorFunction(CollisionClasses::BALL, CollisionClasses::TOPWALL), BallToTopWall });
	ballCollision->CollisionFunctions.insert({ CantorFunction(CollisionClasses::BALL, CollisionClasses::BOTTOMWALL), BallToBottomWall });
	ballCollision->CollisionFunctions.insert({ CantorFunction(CollisionClasses::BALL, CollisionClasses::LEFTWALL), BallToLeftWall });
	ballCollision->CollisionFunctions.insert({ CantorFunction(CollisionClasses::BALL, CollisionClasses::RIGHTWALL), BallToRightWall });
	ballCollision->CollisionFunctions.insert({ CantorFunction(CollisionClasses::BALL, CollisionClasses::PLAYER), BallToPaddle });

	//ai character
	AICharacter = ECSCoord.CreateEntity();
	ECSCoord.AddComponent<C_RigidBody2D>(AICharacter, { glm::vec2((float)Width - 15.0f, 250.0f), 0.0f, glm::vec2(15.0f, 100.0f), true, "paddle" });
	ECSCoord.AddComponent<C_Movement>(AICharacter, { glm::vec2(0.0f, 0.0f), 6.0f, 0.0f });
	ECSCoord.AddComponent<C_Collision>(AICharacter, { CollisionClasses::PLAYER });

}
void Game::ProcessInput(double dt) {
	if (Keys[GLFW_KEY_LEFT_ALT] && Keys[GLFW_KEY_F4]) {
		glfwSetWindowShouldClose(Window, true);
	}
	auto rigidBody = ECSCoord.GetComponent<C_RigidBody2D>(PlayerCharacter);
	auto movement = ECSCoord.GetComponent<C_Movement>(PlayerCharacter);
	if (Keys[GLFW_KEY_UP]) {
		movement->direction = -1.0f;
	}
	if (Keys[GLFW_KEY_DOWN]) {
		movement->direction = 1.0f;
	}
	if (!Keys[GLFW_KEY_DOWN] && !Keys[GLFW_KEY_UP]) {
		movement->direction = 0.0f;
	}
	rigidBody->Position.y += (movement->speed * movement->direction);
}
void Game::Update(double dt) {
	auto ballRigidBody = ECSCoord.GetComponent<C_RigidBody2D>(Ball);
	auto ballMovement = ECSCoord.GetComponent<C_Movement>(Ball);
	ballRigidBody->Position.x += ballMovement->velocity.x;
	ballRigidBody->Position.y += ballMovement->velocity.y;

	auto aiRigidBody = ECSCoord.GetComponent<C_RigidBody2D>(AICharacter);
	auto aiMovement = ECSCoord.GetComponent<C_Movement>(AICharacter);
	if (ballRigidBody->Position.y + (ballRigidBody->Size.y / 2) < aiRigidBody->Position.y + (aiRigidBody->Size.y / 2)) {
		aiMovement->direction = -1.0f;
	}
	else if (ballRigidBody->Position.y + (ballRigidBody->Size.y / 2) > aiRigidBody->Position.y + (aiRigidBody->Size.y / 2)) {
		aiMovement->direction = 1.0f;
	}
	else {
		aiMovement->direction = 0.0f;
	}
	if (abs(ballMovement->velocity.y) < aiMovement->speed) {
		aiRigidBody->Position.y += abs(ballMovement->velocity.y) * aiMovement->direction;
	}
	else {
		aiRigidBody->Position.y += aiMovement->speed * aiMovement->direction;
	}
	ECSCoord.GetSystem<CollisionSystem>()->Update();
}
void Game::Render() {
	ECSCoord.GetSystem<RenderSystem2D>()->Update();
}

void Game::Close() {
	glfwSetWindowShouldClose(Window, true);
}