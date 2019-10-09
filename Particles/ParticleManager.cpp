#include <vector>
#include <iostream>
#include <algorithm>
#include "ParticleManager.h"
#include "Particle.h"
#include "Geometry.h"

ParticleManager::ParticleManager(const float& particleSpawnOffset, const std::shared_ptr<std::vector<Particle>>& particles):
	m_particleSpawnOffset(particleSpawnOffset),
	m_particles(particles)
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::update(const float& dt)
{

	//=== SPAWN PARTICLES CODE ===
	m_currentOffset += dt;
	if(m_currentOffset >= m_particleSpawnOffset)
	{
		m_currentOffset = 0;
	
		spawnParticle(dt);
	}

	//PARTICLES UPDATE
	for (auto it = m_particles->begin(); it != m_particles->end(); ++it)
	{
		it->setLifetime(it->getLifetime() - dt);
		it->updateParticle(dt, Particle::UpdateMethod::EulerSemi);
	}

	m_particles->erase(std::remove_if(m_particles->begin(), m_particles->end(),
		[](Particle& p) { return p.getLifetime() < 0; }), m_particles->end());
}

void ParticleManager::spawnParticle(const float& dt)
{
	Particle newParticle(0 , 5, 0);

	newParticle.setLifetime(15);
	newParticle.setBouncing(0.6);
	newParticle.addForce(0, -9.8f, 0);
	newParticle.setVelocity(
		Geometry::getRandomFloat(-5, 5),
		Geometry::getRandomFloat(-5, 5),
		Geometry::getRandomFloat(-5, 5));
	newParticle.initVerlet(dt);

	m_particles->push_back(newParticle);
}
