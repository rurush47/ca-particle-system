#include <vector>
#include <iostream>
#include <algorithm>
#include "ParticleManager.h"
#include "Particle.h"
#include "Geometry.h"

ParticleManager::ParticleManager(const float& particleSpawnOffset, std::vector<Particle>& particles, bool spawning):
	m_particleSpawnOffset(particleSpawnOffset),
	m_particles(particles),
	m_spawning(spawning)
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::update(const float& dt, const Particle::UpdateMethod& updateMethod)
{

	//=== SPAWN PARTICLES CODE ===
	if(m_spawning)
	{
		m_currentOffset += dt;
		if (m_currentOffset >= m_particleSpawnOffset)
		{
			m_currentOffset = 0;

			spawnParticle(dt);
		}
	}

	//PARTICLES UPDATE
	for (auto it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		it->setLifetime(it->getLifetime() - dt);
		it->updateParticle(dt, updateMethod);
	}

	m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),
		[](Particle& p) { return p.getLifetime() < 0; }), m_particles.end());
}

Particle* ParticleManager::spawnParticle(const float& dt)
{
	Particle newParticle(0 , 5, 0);

	newParticle.setLifetime(900);
	newParticle.setBouncing(0.1);
	newParticle.addForce(0, -9.8f, 0);
	newParticle.setVelocity(
		Geometry::getRandomFloat(-5, 5),
		Geometry::getRandomFloat(-5, 5),
		Geometry::getRandomFloat(-5, 5));
	newParticle.initVerlet(dt);

	m_particles.push_back(newParticle);
	return &m_particles.back();
}

void ParticleManager::clear()
{
	m_particles.clear();
}
