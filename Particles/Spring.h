#pragma once
#include "Particle.h"

class Spring
{
protected:
	Particle* m_p1;
	Particle* m_p2;

	//higher k = stiff spring, lower k = loose spring
	float m_k;//
	float m_b;
	float m_restingLength;

public:
	inline Spring(float k, float b, float length)
		: m_k(k), m_b(b), m_restingLength(length)
	{}

	Particle* getP1();
	Particle* getP2();
	void setParticles(Particle* p1, Particle* p2);
	void setConstants(float k, float b);
	void applyForce(float dt);
	void solveConstraints();
	void render();
};

