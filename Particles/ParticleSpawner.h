#pragma once
#include "Particle.h"
#include <memory>

class ParticleSpawner
{
public:
	ParticleSpawner(const float& particleSpawnOffset, const std::shared_ptr<std::vector<Particle>>& particles);
	~ParticleSpawner();
	void update(const float& dt);
	void spawnParticle();

	float m_particleSpawnOffset;
	float m_currentOffset = 0;
	std::shared_ptr<std::vector<Particle>> m_particleList;
};

