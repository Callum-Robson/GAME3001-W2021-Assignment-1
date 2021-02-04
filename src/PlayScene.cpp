#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include "SoundManager.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	auto deltaTime = TheGame::Instance()->getDeltaTime();
	
	updateDisplayList();

	CollisionManager::AABBCheck(m_pSpaceShip, m_pObstacle);
	CollisionManager::AABBCheck(m_pSpaceShip, m_pTarget);
	if (CollisionManager::lineRectCheck(m_pSpaceShip->getTransform()->position,
		(m_pSpaceShip->getTransform()->position + m_pSpaceShip->getWhiskerOrientation1() * 125.0f),
		m_pObstacle->getTransform()->position, 50.0, 50.0))
	{
		m_pSpaceShip->setWhiskerCollision1(true);
		collisionCheck = true;
	}
	else
	{
		m_pSpaceShip->setWhiskerCollision1(false);
		collisionCheck = false;
	}
	if (CollisionManager::lineRectCheck(m_pSpaceShip->getTransform()->position,
		(m_pSpaceShip->getTransform()->position + m_pSpaceShip->getWhiskerOrientation2() * 125.0f),
		m_pObstacle->getTransform()->position, 50.0, 50.0))
	{
		m_pSpaceShip->setWhiskerCollision2(true);
		collisionCheck = true;
	}
	else
	{
		m_pSpaceShip->setWhiskerCollision2(false);
		collisionCheck = false;
	}
	if (CollisionManager::lineRectCheck(m_pSpaceShip->getTransform()->position,
		(m_pSpaceShip->getTransform()->position + m_pSpaceShip->getOrientation() * 175.0f),
		m_pObstacle->getTransform()->position, 196.0, 189.0))
	{
		m_pSpaceShip->setWhiskerCollision3(true);
		collisionCheck = true;
	}
	else
	{
		m_pSpaceShip->setWhiskerCollision3(false);
		collisionCheck = false; 
	}

	m_pSpaceShip->setObstacleDistance(Util::distance(m_pSpaceShip->getTransform()->position, m_pObstacle->getTransform()->position));
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pObstacle = new Obstacle();
	m_pObstacle->getTransform()->position = glm::vec2(350.0f, 200.0f);
	m_pObstacle->getTransform()->scale = glm::vec2(196.0f, 189.0f);
	m_pObstacle->setEnabled(false);
	addChild(m_pObstacle);

	m_pTarget = new Target();
	m_pTarget->setEnabled(false);
	m_pTarget->getTransform()->position = glm::vec2(700.0f, 300.0f);
	addChild(m_pTarget);
	

	// instantiating spaceship
	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
	m_pSpaceShip->setEnabled(false);
	m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	addChild(m_pSpaceShip);

	m_pInstructionsLabel = new Label("Press 1 for Seeking, 2 for Arrival, 3 for Fleeing", "Consolas", 20, { 255, 0, 0, 255 }, glm::vec2(400.0f, 50.0f));
	m_pInstructionsLabel->setEnabled(false);
	m_pInstructionsLabel->setParent(this);
	addChild(m_pInstructionsLabel);

	m_pInstructionsLabel2 = new Label("4 for Obstacle Avoidance", "Consolas", 20, { 255, 0, 0, 255 }, glm::vec2(420.0f, 69.0f));
	m_pInstructionsLabel2->setEnabled(false);
	m_pInstructionsLabel2->setParent(this);
	addChild(m_pInstructionsLabel2);

	SoundManager::Instance().load("../Assets/audio/wii.mp3", "wii", SOUND_MUSIC);
	SoundManager::Instance().playMusic("wii", -1, MIX_NO_FADING);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	static float speed = 10.0f;
	if(ImGui::SliderFloat("MaxSpeed", &speed, 0.0f, 100.0f))
	{
		m_pSpaceShip->setMaxSpeed(speed);
	}

	static float acceleration_rate = 2.0f;
	if(ImGui::SliderFloat("Acceleration Rate", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pSpaceShip->setAccelerationRate(acceleration_rate);
	}

	static float angleInRadians = m_pSpaceShip->getRotation();
	if(ImGui::SliderAngle("Orientation Angle", &angleInRadians))
	{
		m_pSpaceShip->setRotation(angleInRadians * Util::Rad2Deg);
	}

	static float turn_rate = 5.0f;
	if(ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pSpaceShip->setTurnRate(turn_rate);
	}
	
	if(ImGui::Button("Start"))
	{
		m_pSpaceShip->setEnabled(true);
		m_pTarget->setEnabled(true);
		m_pObstacle->setEnabled(true);
		m_pInstructionsLabel->setEnabled(true);
		m_pInstructionsLabel2->setEnabled(true);
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Reset"))
	{
		m_pSpaceShip->getTransform()->position = glm::vec2(100.0f, 300.0f);
		m_pSpaceShip->setEnabled(false);
		m_pSpaceShip->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pSpaceShip->setRotation(0.0f); // set Angle to 0 degrees
		m_pSpaceShip->setReset(true);
		turn_rate = 5.0f;
		acceleration_rate = 2.0f;
		speed = 10.0f;
		angleInRadians = m_pSpaceShip->getRotation();
	}

	ImGui::Separator();

	static float targetPosition[2] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target", targetPosition, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(targetPosition[0], targetPosition[1]);
		m_pSpaceShip->setDestination(m_pTarget->getTransform()->position);
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
