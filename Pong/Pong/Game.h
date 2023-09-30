#pragma once
#include "RenderSystem2D.h"
#include "CollisionSystem.h"
#include "ECSConstants.h"

#include <memory>

enum GameState {
	MAINMENU_STATE,
	PREGAMEPLAY_STATE,
	GAMEPLAY_STATE,
	POSTGAMEPLAY_STATE
};

class Game {
private:
	GLFWwindow* Window;
	EntityID PlayerCharacter = 0;
	EntityID AICharacter = 0;
	EntityID TopWall = 0;
	EntityID BottomWall = 0;
	EntityID LeftWall = 0;
	EntityID RightWall = 0;
	EntityID Ball = 0;
	
public:
	GameState State;
	bool Keys[1024]{};
	unsigned int Width, Height;
	Game(unsigned int width, unsigned int height);
	~Game();
	void Init(GLFWwindow* window);
	void ProcessInput(double dt);
	void Update(double dt);
	void Render();
	void Close();
};