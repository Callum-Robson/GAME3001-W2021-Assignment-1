#pragma once
#ifndef __SPACE_SHIP__
#define __SPACE_SHIP__
#include "DisplayObject.h"
#include "TextureManager.h"

class SpaceShip final : public DisplayObject
{
public:
	// constructor(s)
	SpaceShip();

	// destructor
	~SpaceShip();
	
	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	void setDestination(glm::vec2 destination);
	void setMaxSpeed(float speed);
	float getMaxSpeed() const;
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation() const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);
	void setReset(bool reset);
	void setWhiskerCollision1(bool collision);
	void setWhiskerCollision2(bool collision);
	void setWhiskerCollision3(bool collision);
	glm::vec2 getWhiskerOrientation1();
	glm::vec2 getWhiskerOrientation2();

private:

	glm::vec2 m_destination; // target
	glm::vec2 m_targetDirection; //normalized length
	glm::vec2 m_orientation; // where ship is facing
	glm::vec2 m_whiskerOrientation1;
	glm::vec2 m_whiskerOrientation2;
	float m_rotationAngle; //adds an angle to -90 offset then sets orientation with this data
	float m_rotationAngle2;
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	int m_behaviorChoice;
	bool m_reset;
	bool m_whiskerCollision1;
	bool m_whiskerCollision2;
	bool m_whiskerCollision3;
	
	// private function
	void m_Move();
};


#endif /* defined (__SPACE_SHIP__) */
