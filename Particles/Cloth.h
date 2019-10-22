#pragma once
#include <glm/detail/type_vec3.hpp>
#include "Spring.h"
#include <vector>

class Cloth
{
protected:
	std::vector<Particle> verts;
	std::vector<Spring> structural;
	std::vector<Spring> shear;
	std::vector<Spring> bend;
	float clothSize;
public:
	// Public API
	void Initialize(int gridSize, float distance, const glm::vec3& position);

	void SetStructuralSprings(float k, float b);
	void SetShearSprings(float k, float b);
	void SetBendSprings(float k, float b);
	void SetParticleMass(float mass);

	// For Physics System
	void applyForces();
	void update(float dt, const Particle::UpdateMethod& updateMethod);
	
	void ApplySpringForces(float dt);
	void render(const Model& model);
	std::vector<Particle>& getVerts();
};


