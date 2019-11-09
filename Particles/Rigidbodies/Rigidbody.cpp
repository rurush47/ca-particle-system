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

void applyImpulse(Rigidbody& rigidbodyA, Rigidbody& rigidbodyB, const CollisionManifold& manifold, int c)
{
	float invMass1 = rigidbodyA.invMass();
	float invMass2 = rigidbodyB.invMass();
	float invMassSum = invMass1 + invMass2;
	if (invMassSum == 0.0f) { return; }

	glm::vec3 relativeV = rigidbodyB.velocity - rigidbodyA.velocity;
	glm::vec3 relativeNorm = manifold.normal;

	//do nothing if objects are moving away from each other
	if (glm::dot(relativeV, relativeNorm) > 0.0f)
	{
		return;
	}

	float e = glm::min(rigidbodyA.cor, rigidbodyB.cor);
	float numerator = -(1.0 + e) * glm::dot(relativeV, relativeNorm);
	//j - magnitute of the impulse
	float j = numerator / invMassSum;

	if (manifold.contacts.size() > 0.0f && j != 0.0f)
	{
		j /= float(manifold.contacts.size());
	}

	glm::vec3 impulse = relativeNorm * j;
	rigidbodyA.velocity = rigidbodyA.velocity - impulse * invMass1;
	rigidbodyB.velocity = rigidbodyB.velocity - impulse * invMass2;

	//Friction
	glm::vec3 tangent = relativeV - (relativeNorm * glm::dot(relativeV, relativeNorm));
	if (glm::abs(glm::length(tangent)) < 0.01f)
	{
		return;
	}

	tangent = glm::normalize(tangent);

	//jt - friction magnitude
	numerator = -glm::dot(relativeV, tangent);
	float jt = numerator / invMassSum;
	if (manifold.contacts.size() > 0.0f && jt != 0.0f)
	{
		jt /= float(manifold.contacts.size());
	}
	if (glm::abs(jt) < 0.01f)
	{
		return;
	}

	//clamp	the	magnitude of friction to between j * friction and j * friction, as shown. Coulomb's Law:
	float friction = glm::sqrt(rigidbodyA.friction * rigidbodyB.friction);
	if (jt > j* friction)
	{
		jt = j * friction;
	}
	else if (jt < -j * friction)
	{
		jt = -j * friction;
	}

	//apply friction
	glm::vec3 tangentImpulse = tangent * jt;

	rigidbodyA.velocity -= tangentImpulse * invMass1;
	rigidbodyB.velocity += tangentImpulse * invMass2;
}

void applyImpulse(Rigidbody& rigidbodyA, Plane& plane, const CollisionManifold& manifold, int c)
{
	float invMass = rigidbodyA.invMass();

	glm::vec3 relativeV = rigidbodyA.velocity;
	glm::vec3 relativeNorm = manifold.normal;

	float e = rigidbodyA.cor;
	float numerator = -(1.0 + e) * glm::dot(relativeV, relativeNorm);
	//j - magnitute of the impulse
	float j = numerator / invMass;

	if (manifold.contacts.size() > 0.0f && j != 0.0f)
	{
		j /= float(manifold.contacts.size());
	}

	glm::vec3 impulse = relativeNorm * j;
	rigidbodyA.velocity = rigidbodyA.velocity - impulse * invMass;

	//TODO Friction
}
