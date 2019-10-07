#include <vector>
#include "ParticleSpawner.h"
#include "Particle.h"
#include <iostream>

ParticleSpawner::ParticleSpawner(const float& particleSpawnOffset, const std::shared_ptr<std::vector<Particle>>& particles):
	m_particleSpawnOffset(particleSpawnOffset),
	m_particleList(particles)
{}

ParticleSpawner::~ParticleSpawner()
{
}

void ParticleSpawner::update(const float& dt)
{
	m_currentOffset += dt;
	if(m_currentOffset >= m_particleSpawnOffset)
	{
		m_currentOffset = 0;

		spawnParticle();
	}
}

void ParticleSpawner::spawnParticle()
{
	Particle newParticle(0 , 5, 0);

	newParticle.setLifetime(900.0f);
	newParticle.setBouncing(0.9);
	newParticle.addForce(0, -9.8f, 0);
	newParticle.setVelocity(-5, 0, -1);

	m_particleList->push_back(newParticle);
}
