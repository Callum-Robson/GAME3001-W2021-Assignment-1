#include "Obstacle.h"


#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance()->load("../Assets/textures/obstacle.png", "obstacle");

	//auto size = TextureManager::Instance()->getTextureSize("obstacle");
	setWidth(196);
	setHeight(189);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(OBSTACLE);
	getRigidBody()->isColliding = false;

	SoundManager::Instance().load("../Assets/audio/yay.wav", "yay", SOUND_SFX);
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	TextureManager::Instance()->draw("obstacle", 
		getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
