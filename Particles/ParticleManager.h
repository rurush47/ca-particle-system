#pragma once
#include <memory>
#include "Particle.h"

class ParticleManager
{
public:
	ParticleManager(const float& particleSpawnOffset, const std::shared_ptr<std::vector<Particle>>& particles);
	~ParticleManager();
	void update(const float& dt);
	void spawnParticle();

	float m_particleSpawnOffset;
	float m_currentOffset = 0;
	const std::shared_ptr<std::vector<Particle>>& m_particles;
	Model m_particleModel;
};

