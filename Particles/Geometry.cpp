#pragma once
#include "Geometry.h"
#include "raylib.h"
#include "rlgl.h"
#include "Particle.h"
#include <functional>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>

//****************************************************
// Plane
//****************************************************

Vector3 Geometry::glmToRaylibVec3(const glm::vec3& vec3)
{
	return Vector3{ vec3.x, vec3.y, vec3.z };
}

float Geometry::getRandomFloat(const float& minVal, const float& maxVal)
{
	return minVal + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxVal - minVal)));
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
}

bool Plane::collides(const glm::vec3& oldPos, const glm::vec3& dtPos)
{
	return (glm::dot(normal, dtPos) + dconst) * (glm::dot(normal, oldPos) + dconst) <= 0;
};

bool Plane::collides(Particle& particle)
{
	auto previousPos = particle.getPreviousPosition();
	auto currentPos = particle.getCurrentPosition();
	return (glm::dot(normal, currentPos) + dconst) * (glm::dot(normal, previousPos) + dconst) <= 0;
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

void Plane::collide(Particle& particle, glm::vec3& previousPosCorrection)
{
	auto bouncing = particle.getBouncing();
	auto previousPos = particle.getPreviousPosition();
	auto currentPos = particle.getCurrentPosition();
	auto currentVelocity = particle.getVelocity();

	//PLANE COLLISION
	auto dotOne = (glm::dot(normal, currentPos) + dconst);
	auto newPos = currentPos - (1 + bouncing) * dotOne * normal;
	 
	//NEW VELOCITY
	auto dotVel = glm::dot(normal, currentVelocity);
	auto newVelocity = currentVelocity - (1 + bouncing) * dotVel * normal;

	//VERLET CORRECTION
	previousPosCorrection = getMirrorPoint(previousPos);

	//SET NEW PARTICLE PARAMETERS
	particle.setPosition(newPos);
	particle.setVelocity(newVelocity);
}

glm::vec3 Plane::getMirrorPoint(const glm::vec3& point)
{
	return point - 2*(glm::dot(normal, point) + dconst) * normal;
}

Triangle::Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2)
{
	vertex1 = point0;
	vertex2 = point1;
	vertex3 = point2;

	normal = -glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
	dconst = -glm::dot(normal, vertex1);

	std::vector<Vector3> vertices;
	vertices.push_back(glmToRaylibVec3(vertex1));
	vertices.push_back(glmToRaylibVec3(vertex2));
	vertices.push_back(glmToRaylibVec3(vertex3));

	m_mesh = genCustomTriangleMesh(vertices, 3, 3);
	m_model = LoadModelFromMesh(m_mesh);
}

Mesh Geometry::genCustomTriangleMesh(const std::vector<Vector3>& customVertices, const int& sides, const float& radius)
{
	Mesh mesh = { 0 };
	auto pointer = (unsigned int*)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));
	std::copy(pointer, pointer + MAX_MESH_VBO, mesh.vboId);
	int vertexCount = sides * 3;

	Vector3* vertices = (Vector3*)RL_MALLOC(vertexCount * sizeof(Vector3));
	for (int i = 0, v = 0; i < 360; i += 360 / sides, v += 3)
	{
		vertices[v] = customVertices[0];
		vertices[v + 1] = customVertices[1];
		vertices[v + 2] = customVertices[2];
	}

	// Normals definition
	Vector3* normals = (Vector3*)RL_MALLOC(vertexCount * sizeof(Vector3));
	for (int n = 0; n < vertexCount; n++) normals[n] = Vector3{ 0.0f, 1.0f, 0.0f };   // Vector3.up;

	// TexCoords definition
	Vector2* texcoords = (Vector2*)RL_MALLOC(vertexCount * sizeof(Vector2));
	for (int n = 0; n < vertexCount; n++) texcoords[n] = Vector2{ 0.0f, 0.0f };

	mesh.vertexCount = vertexCount;
	mesh.triangleCount = sides;
	mesh.vertices = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
	mesh.texcoords = (float*)RL_MALLOC(mesh.vertexCount * 2 * sizeof(float));
	mesh.normals = (float*)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));

	// Mesh vertices position array
	for (int i = 0; i < mesh.vertexCount; i++)
	{
		mesh.vertices[3 * i] = vertices[i].x;
		mesh.vertices[3 * i + 1] = vertices[i].y;
		mesh.vertices[3 * i + 2] = vertices[i].z;
	}

	// Mesh texcoords array
	for (int i = 0; i < mesh.vertexCount; i++)
	{
		mesh.texcoords[2 * i] = texcoords[i].x;
		mesh.texcoords[2 * i + 1] = texcoords[i].y;
	}

	// Mesh normals array
	for (int i = 0; i < mesh.vertexCount; i++)
	{
		mesh.normals[3 * i] = normals[i].x;
		mesh.normals[3 * i + 1] = normals[i].y;
		mesh.normals[3 * i + 2] = normals[i].z;
	}

	RL_FREE(vertices);
	RL_FREE(normals);
	RL_FREE(texcoords);

	// Upload vertex data to GPU (static mesh)
	rlLoadMesh(&mesh, false);

	return mesh;
}

void resetCollisionManifold(CollisionManifold* result)
{
	if (result != 0)
	{
		result->colliding = false;
		result->normal = glm::vec3(0, 0, 1);
		result->depth = FLT_MAX;
		result->contacts.clear();
	}
}

void Triangle::render()
{
	DrawModel(m_model, Vector3{ 0,0,0 }, 1, YELLOW);
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

	bool inside = abs(barycentricValue) < FLOAT_ZERO_COMPARISON_ERROR;
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
// StaticSphere
StaticSphere::StaticSphere(const glm::vec3& point, const float& radius)
{
	center = point;
	rad = radius;
}

void StaticSphere::render()
{
	DrawSphere(glmToRaylibVec3(center), rad, RED);
}

void StaticSphere::setPosition(const glm::vec3& newPos)
{
	center = newPos;
}

bool StaticSphere::isInside(const glm::vec3& point)
{
	return glm::dot(point - center, point - center) <= rad * rad;
}

glm::vec3 StaticSphere::getIntersectionPoint(const glm::vec3& dtPos, const glm::vec3& oldPos)
{
	auto vectorDelta = dtPos - oldPos;
	auto a = glm::dot(vectorDelta, vectorDelta);
	auto b = 2*glm::dot(vectorDelta, (oldPos - center));
	auto c = glm::dot(center, center) + glm::dot(oldPos, oldPos)
		- 2*glm::dot(oldPos, center) - rad * rad;

	//u is an alpha from the course slides
	auto exp = b * b - 4 * a * c;
	auto u1 = (-b + glm::sqrt(exp)) / 2 * a;
	auto u2 = (-b - glm::sqrt(exp)) / 2 * a;
	float u;

	if (u1 >= 0 && u1 <= 1 &&
		u2 >= 0 && u2 <= 1)
	{
		u = glm::min(u1, u2);
	}
	else if(u1 >= 0 && u1 <= 1)
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

std::pair<glm::vec3, glm::vec3> StaticSphere::getCollisionProducts(Particle& particle, const glm::vec3& intersectionPoint)
{
	Plane tangentPlane(intersectionPoint, glm::normalize(intersectionPoint - center));
	return tangentPlane.getCollisionProducts(particle.getCurrentPosition(), particle.getVelocity(), particle.getBouncing());
}

void StaticSphere::collide(Particle& particle, glm::vec3& previousPosCorrection)
{
	const auto intersectionPoint = getIntersectionPoint(particle.getPreviousPosition(), particle.getCurrentPosition());
	Plane tangentPlane(intersectionPoint, glm::normalize(intersectionPoint - center));
	tangentPlane.collide(particle, previousPosCorrection);
}

void applyImpulse(Rigidbody& rigidbodyA, Rigidbody& rigidbodyB, const CollisionManifold& manifold, int c)
{
	float invMass1 = rigidbodyA.invMass();
	float invMass2 = rigidbodyB.invMass();
	float invMassSum = invMass1 + invMass2;
	if (invMassSum == 0.0f) { return; }

	glm::vec3 relativeV = rigidbodyB.velocity - rigidbodyA.velocity;
	glm::vec3 relativeNorm = manifold.normal;

	//do nothing if objects are moving away from each other
	if (glm::dot(relativeV, relativeNorm) > 0.0f)
	{
		return;
	}

	float e = glm::min(rigidbodyA.cor, rigidbodyB.cor);
	float numerator = -(1.0 + e) * glm::dot(relativeV, relativeNorm);
	//j - magnitute of the impulse
	float j = numerator / invMassSum;

	if (manifold.contacts.size() > 0.0f && j != 0.0f)
	{
		j /= float(manifold.contacts.size());
	}

	glm::vec3 impulse = relativeNorm * j;
	rigidbodyA.velocity = rigidbodyA.velocity - impulse * invMass1;
	rigidbodyB.velocity = rigidbodyB.velocity - impulse * invMass2;

	//Friction
	glm::vec3 tangent = relativeV - (relativeNorm * glm::dot(relativeV, relativeNorm));
	if (glm::abs(glm::length(tangent)) < 0.01f)
	{
		return;
	}

	tangent = glm::normalize(tangent);

	//jt - friction magnitude
	numerator = -glm::dot(relativeV, tangent);
	float jt = numerator / invMassSum;
	if (manifold.contacts.size() > 0.0f && jt != 0.0f)
	{
		jt /= float(manifold.contacts.size());
	}
	if (glm::abs(jt) < 0.01f)
	{
		return;
	}

	//clamp	the	magnitude of friction to between j * friction and j * friction, as shown. Coulomb's Law:
	float friction = glm::sqrt(rigidbodyA.friction * rigidbodyB.friction);
	if (jt > j* friction)
	{
		jt = j * friction;
	}
	else if (jt < -j * friction)
	{
		jt = -j * friction;
	}

	//apply friction
	glm::vec3 tangentImpulse = tangent * jt;

	rigidbodyA.velocity -= tangentImpulse * invMass1;
	rigidbodyB.velocity += tangentImpulse * invMass2;
}

void applyImpulse(Rigidbody& rigidbodyA, Plane& plane, const CollisionManifold& manifold, int c)
{
	float invMass = rigidbodyA.invMass();

	glm::vec3 relativeV = rigidbodyA.velocity;
	glm::vec3 relativeNorm = manifold.normal;

	float e = rigidbodyA.cor;
	float numerator = -(1.0 + e) * glm::dot(relativeV, relativeNorm);
	//j - magnitute of the impulse
	float j = numerator / invMass;

	if (manifold.contacts.size() > 0.0f && j != 0.0f)
	{
		j /= float(manifold.contacts.size());
	}

	glm::vec3 impulse = relativeNorm * j;
	rigidbodyA.velocity = rigidbodyA.velocity + impulse * invMass;

	//TODO Friction
}

CollisionManifold findCollisionFeatures(const Sphere& sphereA, const Sphere& sphereB)
{
	CollisionManifold result;
	resetCollisionManifold(&result);

	float r = sphereA.radius + sphereB.radius;
	float distance = glm::distance(sphereB.position, sphereA.position);

	if (distance >= r)
	{
		return result;
	}

	glm::vec3 diffVector = sphereB.position - sphereA.position;
	glm::vec3 n = glm::normalize(diffVector);
	result.colliding = true;
	result.normal = n;
	result.depth = glm::abs(glm::length(diffVector) - r) * 0.5f;

	float distanceToIntersectionPoint = sphereA.radius - result.depth;
	glm::vec3 contact = sphereA.position + diffVector * distanceToIntersectionPoint;
	result.contacts.push_back(contact);

	return result;
}

CollisionManifold findCollisionFeatures(Plane& planeA, const Sphere& sphereB)
{
	CollisionManifold result;
	resetCollisionManifold(&result);

	glm::vec3 closestPoint = planeA.closestPointInPlane(sphereB.position);
	float distance = glm::distance(closestPoint, sphereB.position);
	if (distance >= sphereB.radius)
	{
		return result;
	}

	glm::vec3 normal = glm::normalize(sphereB.position - closestPoint);

	glm::vec3 outsidePoint = sphereB.position - normal * sphereB.radius;
	float collisionDistance = glm::length(closestPoint - outsidePoint);
	result.colliding = true;
	glm::vec3 contact = closestPoint + (outsidePoint - closestPoint) * 0.5f;
	result.contacts.push_back(contact);
	result.normal = normal;
	result.depth = collisionDistance * 0.5;

	return result;
}

//****************************************************

