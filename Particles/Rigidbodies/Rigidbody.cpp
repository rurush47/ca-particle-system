#include "Rigidbody.h"

void Rigidbody::applyForces()
{
	forces = glm::vec3(0, -0.92f, 0) * mass;
}

float Rigidbody::invMass()
{
	if (mass <= 0)
	{
		return 0.0f;
	}
	return 1.0f / mass;
}

void Rigidbody::addLinearImpulse(const glm::vec3 impulse)
{
	velocity = velocity + impulse;
}

void Rigidbody::update(const float& dt)
{
	const float damping = 0.98f;
	glm::vec3 acceleration = forces * invMass();
	velocity = velocity + acceleration * dt;
	velocity = velocity * damping;

	position += velocity;
}