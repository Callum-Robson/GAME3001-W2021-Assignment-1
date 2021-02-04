#include "SpaceShip.h"
#include "EventManager.h"

#include "Game.h"
#include "Util.h"

SpaceShip::SpaceShip()
{
	TextureManager::Instance()->load("../Assets/textures/spaceship.png", "spaceship");

	auto size = TextureManager::Instance()->getTextureSize("spaceship");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SPACE_SHIP);
	setMaxSpeed(10.0f);
	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.1f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);
	setReset(false);

	
	
}

SpaceShip::~SpaceShip()
= default;

void SpaceShip::draw()
{
	TextureManager::Instance()->draw("spaceship", 
		getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	//Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 200.0f) );
	//Util::DrawLine(getTransform()->position, (getTransform()->position + m_whiskerOrientation1 * 150.0f), glm::vec4(1,0,0,0));
	//Util::DrawLine(getTransform()->position, (getTransform()->position + m_whiskerOrientation2 * 150.0f));
}

void SpaceShip::update()
{
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		m_behaviorChoice = 1;
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		m_behaviorChoice = 2;
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3))
	{
		m_behaviorChoice = 3;
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_4))
	{
		m_behaviorChoice = 4;
	}
	
	m_Move();
}

void SpaceShip::clean()
{
}

void SpaceShip::setDestination(const glm::vec2 destination)
{
	m_destination = destination;
}

void SpaceShip::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

float SpaceShip::getMaxSpeed() const
{
	return m_maxSpeed;
}

glm::vec2 SpaceShip::getOrientation() const
{
	return m_orientation;
}

float SpaceShip::getTurnRate() const
{
	return m_turnRate;
}

void SpaceShip::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float SpaceShip::getAccelerationRate() const
{
	return m_accelerationRate;
}

void SpaceShip::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void SpaceShip::setReset(bool reset)
{
	m_reset = reset;
}

void SpaceShip::setWhiskerCollision1(bool collision)
{ 
	m_whiskerCollision1 = collision;
}

void SpaceShip::setWhiskerCollision2(bool collision)
{
	m_whiskerCollision2 = collision;
}

void SpaceShip::setWhiskerCollision3(bool collision)
{
	m_whiskerCollision3 = collision;
}

void SpaceShip::setObstacleDistance(float distance)
{
	m_obstacleDistance = distance;
}

float SpaceShip::get_obstacle_distance()
{
	return m_obstacleDistance;
}

void SpaceShip::setOrientation(const glm::vec2 orientation)
{
	m_orientation = orientation;
}



glm::vec2 SpaceShip::getWhiskerOrientation1()
{
	return m_whiskerOrientation1;
}

glm::vec2 SpaceShip::getWhiskerOrientation2()
{
	return m_whiskerOrientation2;
}


void SpaceShip::setRotation(const float angle)
{
	m_rotationAngle = angle;

	const auto offset = -90.0f;
	const auto offset2 = -70.0f;
	const auto offset3 = -110.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;
	const auto angle_in_radians2 = (angle + offset2) * Util::Deg2Rad;
	const auto angle_in_radians3 = (angle + offset3) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);
	const auto x2 = cos(angle_in_radians2);
	const auto y2 = sin(angle_in_radians2);
	const auto x3 = cos(angle_in_radians3);
	const auto y3 = sin(angle_in_radians3);

	// convert the angle to a normalized vector and store it in Orientation
	setOrientation(glm::vec2(x, y));
	m_whiskerOrientation1 = glm::vec2(x2, y2);
	m_whiskerOrientation2 = glm::vec2(x3, y3);
}

float SpaceShip::getRotation() const
{
	return m_rotationAngle;
}

void SpaceShip::m_Move()
{
	
		auto deltaTime = TheGame::Instance()->getDeltaTime();

		// direction with magnitude
		m_targetDirection = m_destination - getTransform()->position;

		// normalized direction
		m_targetDirection = Util::normalize(m_targetDirection);
		
		if (m_behaviorChoice == 2)
		{
			m_targetDirection *= -1;
		}

		//auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);


		auto turn_sensitivity = 5.0f;
		// this is whats responsible for the ship facing the target and other orientation controls not working
		//if (m_whiskerCollision2 == false && m_whiskerCollision3 == false && m_whiskerCollision1 == false)
			
				auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);
				if (abs(target_rotation) > turn_sensitivity)
				{
					if (target_rotation > 0.0f)
					{
						setRotation(getRotation() + getTurnRate()); // follows by turning left
					}
					else if (target_rotation < 0.0f)
					{
						setRotation(getRotation() - getTurnRate()); // follows target by turning right
					}
				}	
		

		getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	
		if (m_behaviorChoice == 1 || m_behaviorChoice == 2)
		{
			if ((getMaxSpeed() == 0.0 && (Util::distance(getTransform()->position, m_destination) < 40)) || m_reset == false)
			{
				setMaxSpeed(10.0);
				setReset(false);
			}
			// using the formula pf = pi + vi*t + 0.5ai*t^2
			getRigidBody()->velocity += getOrientation() * (deltaTime)+
				0.5f * getRigidBody()->acceleration * (deltaTime);

			getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
			
			getTransform()->position += getRigidBody()->velocity;
		}
		if (m_behaviorChoice == 3)
		{
			if (m_reset == true)
			{
				setMaxSpeed(10.0);
				setReset(false);
			}
			// using the formula pf = pi + vi*t + 0.5ai*t^2
			getRigidBody()->velocity += getOrientation() * (deltaTime)+
				0.5f * getRigidBody()->acceleration * (deltaTime);

			getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
			
				if (Util::distance(getTransform()->position, m_destination) < 300)
				{
					setMaxSpeed(getMaxSpeed() * 0.98);
					if (Util::distance(getTransform()->position, m_destination) < 100)
					{
						setMaxSpeed(getMaxSpeed() * 0.97);
						if (Util::distance(getTransform()->position, m_destination) < 40)
						{
							setMaxSpeed(0.0);
						}
					}

				}
			getTransform()->position += getRigidBody()->velocity;
		}


	
		if (m_behaviorChoice == 4)
		{
			/*if ((getMaxSpeed() == 0.0 && (Util::distance(getTransform()->position, m_destination) < 40)) || m_reset == true)
			{
				setMaxSpeed(10.0);
				setReset(false);
			}*/
			if (m_whiskerCollision3 == true)// && m_whiskerCollision2 == false && m_whiskerCollision1 == false
			{
				if (m_maxSpeed > 2)
				setMaxSpeed(getMaxSpeed() * 0.96);
				getRigidBody()->velocity.y += 0.70;
				
			}
			if (m_whiskerCollision1 == true)
			{
				//setMaxSpeed(1.0);
				getRigidBody()->velocity.y += 1.25;
				//setRotation(0);
			}
			else if (m_whiskerCollision2 == true)
			{
				//setMaxSpeed(1.0);
				getRigidBody()->velocity.y += 1.25;
				//setRotation(180);
			}
			/*if (m_obstacleDistance > 20 && m_whiskerCollision1 == false)
			{
				setRotation(90);
			}
			if (m_obstacleDistance > 20 && m_whiskerCollision2 == false);
			{
				setRotation(90);
			}*/
			
			// using the formula pf = pi + vi*t + 0.5ai*t^2
			getRigidBody()->velocity += getOrientation() * (deltaTime)+
				0.5f * getRigidBody()->acceleration * (deltaTime);

			getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxSpeed);
			
			getTransform()->position += getRigidBody()->velocity;
		}
	
}
