#include "Cloth.h"

void Cloth::Initialize(int gridSize, float distance, const glm::vec3& position) {
	float k = 20;
	float b = 0.8f;
	clothSize = gridSize;

	verts.clear();
	structural.clear();
	shear.clear();
	bend.clear();

	verts.resize(gridSize * gridSize);
	float halfSize = (float)(gridSize - 1) * 0.5f;

	if (gridSize < 3) {
		gridSize = 3;
	}

	// Create vertices
	for (int x = 0; x < gridSize; ++x) {
		for (int z = 0; z < gridSize; ++z) {
			int i = z * gridSize + x;

			float x_pos = ((float)x + position.x - halfSize) * distance;
			float z_pos = ((float)z + position.z - halfSize) * distance;

			verts[i] = *new Particle(x_pos, position.y, z_pos);
			//verts[i].setPosition(glm::vec3(x_pos, position.y, z_pos));
			verts[i].setMass(1.0f);
			verts[i].setBouncing(0.0f);
			verts[i].setFriction(0.01f);
			verts[i].addForce(0, -9.8f, 0);
		}
	}

	for (int i = 0; i < gridSize; ++i)
	{
		verts[i].setFixed(true);
	}

	// Create left to right structural springs
	for (int x = 0; x < gridSize; ++x) {
		for (int z = 0; z < gridSize - 1; ++z) {
			int i = z * gridSize + x;
			int j = (z + 1) * gridSize + x;

			float rest = glm::length(verts[i].getCurrentPosition() - verts[j].getCurrentPosition());
			Spring spring(k, b, rest);
			spring.setParticles(&verts[i], &verts[j]);
			structural.push_back(spring);
		}
	}

	// Create up and down structural springs
	for (int x = 0; x < gridSize - 1; ++x) {
		for (int z = 0; z < gridSize; ++z) {
			int i = z * gridSize + x;
			int j = z * gridSize + (x + 1);

			float rest = glm::length(verts[i].getCurrentPosition() - verts[j].getCurrentPosition());
			Spring spring(k, b, rest);
			spring.setParticles(&verts[i], &verts[j]);
			structural.push_back(spring);
		}
	}

	// Create left to right shear springs
	for (int x = 0; x < gridSize - 1; ++x) {
		for (int z = 0; z < gridSize - 1; ++z) {
			int i = z * gridSize + x;
			int j = (z + 1) * gridSize + (x + 1);

			float rest = glm::length(verts[i].getCurrentPosition() - verts[j].getCurrentPosition());
			Spring spring(k, b, rest);
			spring.setParticles(&verts[i], &verts[j]);
			shear.push_back(spring);
		}
	}

	// Create right to left shear springs
	for (int x = 1; x < gridSize; ++x) {
		for (int z = 0; z < gridSize - 1; ++z) {
			int i = z * gridSize + x;
			int j = (z + 1) * gridSize + (x - 1);

			float rest = glm::length(verts[i].getCurrentPosition() - verts[j].getCurrentPosition());
			Spring spring(k, b, rest);
			spring.setParticles(&verts[i], &verts[j]);
			shear.push_back(spring);
		}
	}

	// Create left to right bend springs
	for (int x = 0; x < gridSize; ++x) {
		for (int z = 0; z < gridSize - 2; ++z) {
			int i = z * gridSize + x;
			int j = (z + 2) * gridSize + x;

			float rest = glm::length(verts[i].getCurrentPosition() - verts[j].getCurrentPosition());
			Spring spring(k, b, rest);
			spring.setParticles(&verts[i], &verts[j]);
			bend.push_back(spring);
		}
	}

	// Create up and down bend springs
	for (int x = 0; x < gridSize - 2; ++x) {
		for (int z = 0; z < gridSize; ++z) {
			int i = z * gridSize + x;
			int j = z * gridSize + (x + 2);

			float rest = glm::length(verts[i].getCurrentPosition() - verts[j].getCurrentPosition());
			Spring spring(k, b, rest);
			spring.setParticles(&verts[i], &verts[j]);
			bend.push_back(spring);
		}
	}
}

void Cloth::SetStructuralSprings(float k, float b) {
	for (int i = 0, size = structural.size(); i < size; ++i) {
		structural[i].setConstants(k, b);
	}
}

void Cloth::SetShearSprings(float k, float b) {
	for (int i = 0, size = shear.size(); i < size; ++i) {
		shear[i].setConstants(k, b);
	}
}

void Cloth::SetBendSprings(float k, float b) {
	for (int i = 0, size = bend.size(); i < size; ++i) {
		bend[i].setConstants(k, b);
	}
}

void Cloth::SetParticleMass(float mass) {
	for (int i = 0, size = verts.size(); i < size; ++i) {
		verts[i].setMass(mass);
	}
}

void Cloth::applyForces() {
	// for (int i = 0, size = verts.size(); i < size; ++i) {
	// 	verts[i].applyForces();
	// }
}

void Cloth::update(float dt, const Particle::UpdateMethod& updateMethod) {
	for (int i = 0, size = verts.size(); i < size; ++i) {
		verts[i].updateParticle(dt, updateMethod);
	}
}

void Cloth::ApplySpringForces(float dt) {
	for (int i = 0, size = structural.size(); i < size; ++i) {
		structural[i].applyForce(dt);
	}
	for (int i = 0, size = shear.size(); i < size; ++i) {
		shear[i].applyForce(dt);
	}
	for (int i = 0, size = bend.size(); i < size; ++i) {
		bend[i].applyForce(dt);
	}
}

void Cloth::render(const Model& model)
{
	for (Particle& particle : verts)
	{
		particle.render(model);
	}

	for (int i = 0, size = structural.size(); i < size; ++i) {
		structural[i].render();
	}
	for (int i = 0, size = shear.size(); i < size; ++i) {
		shear[i].render();
	}
	for (int i = 0, size = bend.size(); i < size; ++i) {
		bend[i].render();
	}
}

std::vector<Particle>& Cloth::getVerts()
{
	return verts;
}
