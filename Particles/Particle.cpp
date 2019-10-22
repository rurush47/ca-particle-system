#include "Particle.h"
#include <raylib.h>
#include "Geometry.h"
#include <iostream>
#include <string>


Particle::Particle()
{
}

Particle::Particle(const float& x, const float& y, const float& z) :
m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_mass(1), m_fixed(false), m_friction(0)
{
	m_currentPosition.x = x;
	m_currentPosition.y = y;
	m_currentPosition.z = z;

	m_previousPosition.x = x;
	m_previousPosition.y = y;
	m_previousPosition.z = z;
}

/*
Particle::Particle(glm::vec3 pos, glm::vec3 vel, float bouncing, bool fixed, int lifetime, glm::vec3 force) :
m_currentPosition(pos), m_previousPosition(pos), m_force(force), m_velocity(vel), m_bouncing(bouncing), m_lifetime(lifetime), m_fixed(fixed)
{
}
*/

Particle::~Particle()
{
}

//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos(x,y,z);
	m_currentPosition =  pos;
}
void Particle::setPosition(glm::vec3 pos)
{
	if(m_fixed)
	{
		return;
	}
	
	m_currentPosition = pos;
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos(x, y, z);
	m_previousPosition = pos;
}

void Particle::setPreviousPosition(glm::vec3 pos)
{
	m_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force(x, y, z);
	m_force = force;
}

void Particle::setForce(glm::vec3 force)
{
	m_force = force;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force(x,y,z);
	m_force += force;
}

void Particle::addForce(glm::vec3 force)
{
	m_force += force;
}

void Particle::setVelocity(const float& x, const float& y, const float& z)
{
	if(m_fixed)
	{
		return;
	}
	
	glm::vec3 vel(x,y,z);
	m_velocity = vel;
}

void Particle::setVelocity(glm::vec3 vel)
{
	m_velocity = vel;
}

void Particle::setBouncing(float bouncing)
{
	m_bouncing = bouncing;
}

void Particle::setLifetime(float lifetime)
{
	m_lifetime = lifetime;
}

void Particle::setFixed(bool fixed)
{
	m_fixed = fixed;
	//== SET FORCES TO ZERO ===
	setPreviousPosition(m_currentPosition);
	m_velocity = glm::vec3(0, 0, 0);
	//m_force = glm::vec3(0, 0, 0);
}

void Particle::setMass(float mass)
{
	if(mass < 0)
	{
		mass = 0;
	}
	m_mass = mass;
}

//getters
glm::vec3 Particle::getCurrentPosition()
{
	return m_currentPosition;
}

Vector3 * Particle::getCurrentPositionRaylib()
{

	return new Vector3{ m_currentPosition.x, m_currentPosition.y, m_currentPosition.z };
}


glm::vec3 Particle::getPreviousPosition()
{
	return m_previousPosition;
}

glm::vec3 Particle::getForce()
{
	return m_force;
}

glm::vec3 Particle::getVelocity()
{
	return m_velocity;
}

float Particle::getBouncing()
{
	return m_bouncing;
}

float Particle::getLifetime()
{
	return m_lifetime;
}

bool Particle::isFixed()
{
	return m_fixed;
}

float Particle::getMass()
{
	return m_mass;
}

glm::vec3 Particle::getAllForces()
{
	return m_force + m_impulse;
}

float Particle::getFriction()
{
	return m_friction;
}

void Particle::updateParticle(const float& dt, UpdateMethod method)
{
	if (!m_fixed & m_lifetime > 0)
	{
		switch (method)
		{
		case UpdateMethod::EulerOrig:
		{
			m_previousPosition = m_currentPosition;
			m_currentPosition += m_velocity*dt;
			const auto acc = getAllForces() * invMass();
			m_velocity += acc*dt;
		}
			break;
		case UpdateMethod::EulerSemi:
		{
			m_previousPosition = m_currentPosition;
			const auto acc = getAllForces()* invMass();
			m_velocity += acc*dt;
			m_currentPosition += m_velocity*dt;
		}
			break;
		case UpdateMethod::Verlet:
		{
			const auto pos = m_currentPosition;
			const auto acc = getAllForces() * invMass();
			m_currentPosition = 2.0f*m_currentPosition - m_previousPosition + (dt*dt)*(acc);
			m_previousPosition = pos;
				
			m_velocity = (m_currentPosition - m_previousPosition) / dt;
		}
			break;
		}
	}

	m_impulse = glm::vec3(0, 0, 0);
	
	return;
}

void Particle::render(const Model& model)
{
	DrawModel(model, Geometry::glmToRaylibVec3(m_currentPosition), 1, GREEN);
}

void Particle::initVerlet(const float& dt)
{
	m_previousPosition = m_currentPosition - m_velocity * dt;
}

glm::vec3 m_impulse;

void Particle::addImpulse(const glm::vec3& impulse)
{
	// if(m_fixed)
	// {
	// 	return;
	// }

	m_impulse += impulse;
}

float Particle::invMass()
{
	if(m_mass == 0.0f)
	{
		return 0.0f;
	}
	return 1.0f / m_mass;
}

void Particle::setFriction(float friction)
{
	if(friction < 0)
	{
		friction = 0;
	}
	m_friction = friction;
}

void Particle::debugPosition()
{
	std::cout << std::to_string(m_currentPosition.x) + " " + std::to_string(m_currentPosition.y) + " " + std::to_string(m_currentPosition.z) << std::endl;
}
