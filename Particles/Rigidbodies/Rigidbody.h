#pragma once
#include <glm/detail/type_vec3.hpp>

class Rigidbody
{
public:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 forces;
	float mass;
	float cor; // Coefficient of restitution
	float friction;

	Rigidbody() :mass(1.0f), cor(1), friction(0.6f) {}
	~Rigidbody() {}

	virtual void render() {}
	void update(const float& dt);
	void applyForces();
	float invMass();
	void addLinearImpulse(const glm::vec3 impulse);
};