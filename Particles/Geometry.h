#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <glm\glm.hpp>
#include <utility>
#include "raylib.h"
#include "Particle.h"

struct Geometry{
	virtual void setPosition(const glm::vec3& newPos) = 0;
	virtual bool isInside(const glm::vec3& point) = 0;
	static Vector3 glmToRaylibVec3(const glm::vec3& vec3);
};

struct Plane : public Geometry {
	const float size = 5;
	glm::vec3 normal;
	float dconst;
	Plane(){};
	~Plane() {};
	Plane(const glm::vec3& point, const glm::vec3& normalVect);
	Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

	void setPosition(const glm::vec3& newPos);
	bool isInside(const glm::vec3& point);
	float distPoint2Plane(const glm::vec3& point);
	glm::vec3 closestPointInPlane(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
	std::pair<glm::vec3, glm::vec3> getCollisionProducts(const glm::vec3& pos, const glm::vec3& velocity, const float& bouncing);
};	

struct Triangle : public Plane {
	//glm::vec3 vertex1, vertex2, vertex3;
	//Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);
	//~Triangle() {};
	//void setPosition(const glm::vec3& newPos);
	//bool isInside(const glm::vec3& point);
	//bool intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall);
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
