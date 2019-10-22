#pragma once
#include <memory>
#include "Particle.h"

class ParticleManager
{
public:
	ParticleManager(const float& particleSpawnOffset, std::vector<Particle>& particles, bool spawning = true);
	~ParticleManager();
	void update(const float& dt, const Particle::UpdateMethod& updateMethod);
	Particle* spawnParticle(const float& dt);
	void clear();

	bool m_spawning;
	float m_particleSpawnOffset;
	float m_currentOffset = 0;
	std::vector<Particle>& m_particles;
	Model m_particleModel;
};

