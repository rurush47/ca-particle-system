#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <glm\glm.hpp>
#include "raylib.h"

class Particle
{
public:
	enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };

	Particle();
	Particle(const float& x, const float& y, const float& z);
//	Particle(glm::vec3 pos, glm::vec3 vel, float bouncing = 1.0f, bool fixed = false, int lifetime = -1, glm::vec3 force = glm::vec3(0, 0, 0));
	~Particle();
	//setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos);
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos);
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel);
	void setAcceleration(const float& x, const float& y, const float& z);
	void setAcceleration(glm::vec3 force);
	void setBouncing(float bouncing);
	void setLifetime(float lifetime);
	void setFixed(bool fixed);
	void setMass(float mass);

	//getters
	glm::vec3 getCurrentPosition();
	Vector3 * getCurrentPositionRaylib();
	glm::vec3 getPreviousPosition();
	glm::vec3 getAcceleration();
	glm::vec3 getVelocity();
	float getBouncing();
	float getLifetime();
	bool isFixed();
	float getMass();

	//other
	void addAcceleration(glm::vec3 acceleration);
	void addAcceleration(const float& x, const float& y, const float& z);
	void updateParticle(const float& dt, UpdateMethod method = UpdateMethod::EulerOrig);
	void render(const Model& model);
	void initVerlet(const float& dt);

private:
	glm::vec3 m_currentPosition;
	glm::vec3 m_previousPosition;
	glm::vec3 m_acceleration;
	glm::vec3 m_velocity;
	float m_mass;

	float m_bouncing;
	float m_lifetime;
	bool  m_fixed;

};

#endif