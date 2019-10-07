#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#define MAX_MESH_VBO 7
#define FLOAT_ZERO_COMPARISON_ERROR 0.01f


#include <glm\glm.hpp>
#include <utility>
#include "raylib.h"
#include "Particle.h"
#include <iosfwd>
#include <vector>

struct Geometry{
	virtual void setPosition(const glm::vec3& newPos) = 0;
	virtual bool isInside(const glm::vec3& point) = 0;
	static Vector3 glmToRaylibVec3(const glm::vec3& vec3);
	static float getRandomFloat(const float& minVal, const float& maxVal);
	Mesh genCustomTriangleMesh(const std::vector<Vector3>& customVertices, const int& sides, const float& radius);
};

struct Plane : public Geometry {
	const float size = 5;
	glm::vec3 normal;
	float dconst;
	Plane(){};
	~Plane() {};
	Plane(const glm::vec3& point, const glm::vec3& normalVect);
	Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

	bool collides(const glm::vec3& oldPos, const glm::vec3& dtPos);
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	float distPoint2Plane(const glm::vec3& point);
	glm::vec3 closestPointInPlane(const glm::vec3& point);
	virtual bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
	std::pair<glm::vec3, glm::vec3> getCollisionProducts(const glm::vec3& pos, const glm::vec3& velocity, const float& bouncing);
};	

struct Triangle : public Plane
{
private:
	Mesh m_mesh;
	Model m_model;
public:
	glm::vec3 vertex1, vertex2, vertex3;
	Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);
	~Triangle() {};

	void render();
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& onPlanePoint);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
	float getBarycentricProduct(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
};

struct Sphere : public Geometry {
	glm::vec3 center;
	float rad;
	Sphere(const glm::vec3& point, const float& radius);
	~Sphere() {}
	void render();
	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	glm::vec3 getIntersectionPoint(const glm::vec3& dtPos, const glm::vec3& oldPos);
	std::pair<glm::vec3, glm::vec3> getCollisionProducts(Particle& particle, const glm::vec3& intersectionPoint);
	//bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
};

#endif
