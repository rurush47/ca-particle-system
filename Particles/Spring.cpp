#include "Spring.h"
#include "Geometry.h"
#include <iostream>
#include <string>

Particle* Spring::getP1()
{
	return m_p1;
}

Particle* Spring::getP2()
{
	return m_p2;
}

void Spring::setParticles(Particle* p1, Particle* p2)
{
	m_p1 = p1;
	m_p2 = p2;
}

void Spring::setConstants(float k, float b)
{
	m_k = k;
	m_b = b;
}

void Spring::applyForce(float dt)
{
	glm::vec3 relPos = m_p2->getCurrentPosition() - m_p1->getCurrentPosition();
	glm::vec3 relVel = m_p2->getVelocity() - m_p1->getVelocity();

	float relPLength = glm::length(relPos);
	float relVLength = glm::length(relVel);

	float one = m_k * (relPLength - m_restingLength);
	float two = m_b * glm::dot(relVel, glm::normalize(relPos));
	glm::vec3 impulse = (one + two) * glm::normalize(relPos);

	m_p1->addImpulse(impulse);
	m_p2->addImpulse(impulse * -1.0f);
}

void Spring::solveConstraints()
{
	glm::vec3 delta = m_p2->getCurrentPosition() - m_p1->getCurrentPosition();
	float distance = glm::length(delta);
	float correction = (distance - m_restingLength) / distance;

	m_p1->setPosition(m_p1->getCurrentPosition() + delta * 0.5f * correction);
	m_p2->setPosition(m_p2->getCurrentPosition() - delta * 0.5f * correction);
}

void Spring::render()
{
	DrawLine3D(Geometry::glmToRaylibVec3(m_p1->getCurrentPosition()), Geometry::glmToRaylibVec3(m_p2->getCurrentPosition()), GREEN);
}
