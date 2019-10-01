#pragma once
#include "Geometry.h"
#include "raylib.h"
#include "Particle.h"
#include <functional>

#define FLOAT_ZERO_COMPARISON_ERROR 0.01f

//****************************************************
// Plane
//****************************************************

Vector3 Geometry::glmToRaylibVec3(const glm::vec3& vec3)
{
	return Vector3{ vec3.x, vec3.y, vec3.z };
}

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect){
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(point, normal);
};

Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
	glm::vec3 v1 = point1 - point0;
	glm::vec3 v2 = point2 - point0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(point0, normal);
};

void Plane::setPosition(const glm::vec3& newPos){
	dconst = -glm::dot(newPos, normal);
};

bool Plane::isInside(const glm::vec3& point){
	float dist;
	dist = glm::dot(point, normal) + dconst;
	if (dist > 1.e-7)
		return false;
	else
		return true;
};

float Plane::distPoint2Plane(const glm::vec3& point){
	float dist;
	return dist = glm::dot(point, normal) + dconst;
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point){
	glm::vec3 closestP;
	float r = (-dconst - glm::dot(point, normal));
	return closestP = point + r*normal;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall) {
	if (distPoint2Plane(point1) * distPoint2Plane(point2) > 0)	return false;
	float r = (-dconst - glm::dot(point1, normal)) / glm::dot((point2 - point1), normal);
	pTall = (1 - r) * point1 + r * point2;
	return true;
}

std::pair<glm::vec3, glm::vec3> Plane::getCollisionProducts(const glm::vec3& pos, const glm::vec3& velocity, const float& bouncing)
{
	//PLANE COLLISION (proper one)
	auto currentPos = pos;
	auto dotOne = (glm::dot(normal, currentPos) + dconst);
	auto newPos = currentPos - (1 + bouncing) * dotOne * normal;

	//NEW VELOCITY (proper one)
	auto currentVelocity = velocity;
	auto dotVel = glm::dot(normal, currentVelocity);
	auto newVelocity = currentVelocity - (1 + bouncing) * dotVel * normal;

	return std::pair<glm::vec3, glm::vec3>{newPos, newVelocity};
}

Triangle::Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2)
{
	vertex1 = point0;
	vertex2 = point1;
	vertex3 = point2;

	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
	dconst = -glm::dot(normal, vertex1);
}

void Triangle::render()
{
	DrawLine3D(glmToRaylibVec3(vertex1), glmToRaylibVec3(vertex2), BLUE);
	DrawLine3D(glmToRaylibVec3(vertex2), glmToRaylibVec3(vertex3), BLUE);
	DrawLine3D(glmToRaylibVec3(vertex3), glmToRaylibVec3(vertex1), BLUE);
}

void Triangle::setPosition(const glm::vec3& newPos)
{
	//TODO implement
}

bool Triangle::isInside(const glm::vec3& onPlanePoint)
{
	float barycentricValue =
		getBarycentricProduct(onPlanePoint, vertex2, vertex3) +
		getBarycentricProduct(vertex1, onPlanePoint, vertex3) +
		getBarycentricProduct(vertex1, vertex2, onPlanePoint) -
		getBarycentricProduct(vertex1, vertex2, vertex3);

	bool inside = glm::abs(barycentricValue) < FLOAT_ZERO_COMPARISON_ERROR;
	return inside;
}

bool Triangle::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall)
{
	return Plane::intersecSegment(point1, point2, pTall);
}

float Triangle::getBarycentricProduct(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
	auto cross = glm::cross(v2 - v1, v3 - v1);
	auto length = glm::length(cross);
	auto val = length / 2.0f;
	return val;
}

// void Plane::render()
// {
// 	auto point1 = normal*dconst
// 	DrawLine3D();
// 	DrawLine3D();
// 	DrawLine3D();
// 	DrawLine3D();
// };


//****************************************************
// Triangle
//****************************************************


//****************************************************
// Sphere
Sphere::Sphere(const glm::vec3& point, const float& radius)
{
	center = point;
	rad = radius;
}

void Sphere::render()
{
	DrawSphere(glmToRaylibVec3(center), rad, RED);
}

void Sphere::setPosition(const glm::vec3& newPos)
{
	center = newPos;
}

bool Sphere::isInside(const glm::vec3& point)
{
	return glm::dot(point - center, point - center) <= rad * rad;
}

glm::vec3 Sphere::getIntersectionPoint(const glm::vec3& dtPos, const glm::vec3& oldPos)
{
	auto vectorDelta = dtPos - oldPos;
	auto a = glm::dot(vectorDelta, vectorDelta);
	auto b = glm::dot(vectorDelta * 2.0f, (oldPos - center));
	auto c = glm::dot(center, center) + glm::dot(oldPos, oldPos)
		- glm::dot(2.0f * oldPos, center) - rad * rad;

	//u is an alpha from the course slides
	auto u1 = (-b + glm::sqrt(b * b - 4 * a * c)) / 2 * a;
	auto u2 = (-b - glm::sqrt(b * b - 4 * a * c)) / 2 * a;
	float u;

	if(u1 >= 0 && u1 <= 1)
	{
		u = u1;
	}
	else if(u2 >= 0 && u2 <= 1)
	{
		u = u2;
	}
	else
	{
		throw "Segment doesn't intersect the sphere! sCheck the collision detection code!";
	}

	return oldPos + u * vectorDelta;
}

std::pair<glm::vec3, glm::vec3> Sphere::getCollisionProducts(Particle& particle, const glm::vec3& intersectionPoint)
{
	//TODO remove plane ?
	Plane tangentPlane(intersectionPoint, glm::normalize(intersectionPoint - center));
	return tangentPlane.getCollisionProducts(particle.getCurrentPosition(), particle.getVelocity(), particle.getBouncing());
}

//****************************************************

