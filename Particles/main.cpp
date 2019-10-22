#include <iostream>
#include <vector>
#include <string>
#include "Particle.h"
#include "Geometry.h"
#include "raylib.h"
#include "camera.h"
#include "ParticleManager.h"
#include "Spring.h"
#include "Cloth.h"

enum class SimulationMode
{
	Cloth,
	Rope
};

void main(){

	//===RAYLIB===
	//// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

	// Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = *new Vector3{ 10.0f, 10.0f, 10.0f }; // Camera position
	camera.target = *new Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = *new Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 60.0f;                                // Camera field-of-view Y
	camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a free camera mode

	int targetFPS = 120;
	SetTargetFPS(targetFPS);                   // Set our game to run at 60 frames-per-second


	//=== CONSTS ===
	Particle::UpdateMethod updateMethod = Particle::UpdateMethod::EulerSemi;
	float dt = 0.005;  //simulation time
	float tini = 0.0f; 
	//================

	//=== INIT PARTICLES MANAGER ===
	std::vector<Particle> particles;
	ParticleManager ps(0.06f, particles, false);
	auto particleModel = LoadModelFromMesh(GenMeshSphere(0.07f, 6, 6));



	//=== SPRING ===
	std::vector<Spring> springs;
	//==============================

	//=== CLOTH ===
	Cloth cloth;
	//=============
	
	//=== DEFINE PLANE BOX ==========
	glm::vec3 punt1(0, 0, 0);
	glm::vec3 punt2(5, 0, 0);
	glm::vec3 punt3(-5, 0, 0);
	glm::vec3 punt4(0, 0, 5);
	glm::vec3 punt5(5, 0, -5);
	glm::vec3 normal1(0, 1, 0);
	glm::vec3 normal2(-1, 0, 0);
	glm::vec3 normal3(1, 0, 0);
	glm::vec3 normal4(0, 0, -1);
	glm::vec3 normal5(0, 0, 1);
	Plane plane1(punt1, normal1);
	Plane plane2(punt2, normal2);
	Plane plane3(punt3, normal3);
	Plane plane4(punt4, normal4);
	Plane plane5(punt5, normal5);
	std::vector<Plane> planes;
	planes.push_back(plane1);
	planes.push_back(plane2);
	planes.push_back(plane3);
	planes.push_back(plane4);
	planes.push_back(plane5);
	//===============================

	//=== DEFINE SPHERE ==============
	glm::vec3 sphereCenter(-3.0f, 3.5f, 0.6f);
	Sphere sphere(sphereCenter, 3);
	//================================

	//=== DEFINE TRIANGLE=============
	glm::vec3 triangleV1(5, 0, 0);
	glm::vec3 triangleV2(5, 5, -5);
	glm::vec3 triangleV3(0, 0, -5);
	Triangle triangle(triangleV1, triangleV2, triangleV3);
	//================================
	
	// simulation loop
	int count = 0;
	float time = tini;
	bool collides;
	glm::vec3 previousPosCorrection;
	// Main game loop

	bool startLoop = false;

	SimulationMode simulationMode = SimulationMode::Cloth;

	while (!WindowShouldClose())        // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera);          // Update camera

		if (IsKeyDown('Z')) camera.target = *new Vector3{ 0.0f, 0.0f, 0.0f };

		if (IsKeyDown('C'))
		{
			simulationMode = SimulationMode::Cloth;

			cloth.Initialize(8, 0.5, glm::vec3(0, 10, 0));
		}

		if (IsKeyDown('R'))
		{
			simulationMode = SimulationMode::Rope;

			ps.clear();
			ps.spawnParticle(dt);
			ps.spawnParticle(dt);
			ps.spawnParticle(dt);
			ps.spawnParticle(dt);
			ps.spawnParticle(dt);
			Particle* p1 = &particles[0];
			Particle* p2 = &particles[1];
			Particle* p3 = &particles[2];
			Particle* p4 = &particles[3];
			Particle* p5 = &particles[4];

			p1->setFixed(true);
			p1->setPosition(0, 10, 0);

			p2->setPosition(1, 9, 0);

			p3->setPosition(2, 8, 0);

			p4->setPosition(3, 7, 0);

			p5->setPosition(4, 6, 0);

			Spring s(20, 0.8f, 1.5f);
			s.setParticles(p1, p2);
			springs.push_back(s);

			Spring s2(20, 0.8f, 1.5f);
			s2.setParticles(p2, p3);
			springs.push_back(s2);

			Spring s3(20, 0.8f, 1.5f);
			s3.setParticles(p3, p4);
			springs.push_back(s3);

			Spring s4(20, 0.8f, 1.5f);
			s4.setParticles(p4, p5);
			springs.push_back(s4);
		}
		
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);
		
		BeginMode3D(camera);


		if(simulationMode == SimulationMode::Rope)
		{
			//=== SPRINGS UPDATE ===
			for (Spring& spring : springs)
			{
				spring.applyForce(dt);
			}
			//======================

			for (Spring& spring : springs)
			{
				spring.solveConstraints();
			}
			// === CLOTH UPDATE ===
		}
		
		if (simulationMode == SimulationMode::Cloth)
		{
			cloth.ApplySpringForces(dt);

			cloth.update(dt, updateMethod);
		}
		//=====================
		
		//===UPDATE PARTICLES===
		ps.update(dt, updateMethod);
		//======================

		std::vector<Particle>* partilcesVector;
		if(simulationMode == SimulationMode::Rope)
		{
			partilcesVector = &particles;
		}
		else
		{
			partilcesVector = &cloth.getVerts();
		}
		
		//
		for (Particle& particle : *partilcesVector)
		{
			collides = false;
		
			//=== PLANES COLLISION ===
			for (Plane& plane : planes)
			{
				bool planeCollision = plane.collides(particle);
				collides = collides || planeCollision;
				if (planeCollision)
				{
					plane.collide(particle, previousPosCorrection);
					//particle.setPreviousPosition(previousPosCorrection);
				}
			}
			//========================
			
			//=== SPHERE COLLISION ===
			bool sphereCollision = sphere.isInside(particle.getCurrentPosition());
			collides = collides || sphereCollision;
			if (sphereCollision)
			{
				sphere.collide(particle, previousPosCorrection);
				//particle.setPreviousPosition(previousPosCorrection);
			}
			//=========================
		
			//=== TRIANGLE COLLISION ===
			glm::vec3 intersectionVec;
			bool triangleCollision = triangle.intersecSegment(particle.getPreviousPosition(), particle.getCurrentPosition(), intersectionVec) &&
				triangle.isInside(intersectionVec);
			collides = collides || triangleCollision;
			if (triangleCollision)
			{
				triangle.collide(particle, previousPosCorrection);
				//particle.setPreviousPosition(previousPosCorrection);
			}
			//==========================
		
			//UPDATE PREVIOUS POS FOR VERLET (FROM LAST COLLISION)
			if(collides && updateMethod == Particle::UpdateMethod::Verlet)
			{
				particle.setPreviousPosition(previousPosCorrection);
			}
			//====================================================
			//=== PARTICLE RENDER ===
			particle.render(particleModel);
			//========================
		}
		//=== OBJECTS RENDERING

		if(simulationMode == SimulationMode::Rope)
		{
			for (Spring& spring : springs)
			{
				spring.render();
			}
		}
		else
		{
			cloth.render(particleModel);
		}
		
		sphere.render();
		triangle.render();
		//================

		//DRAW SCENE BOUNDS------------------------------------------------------------------
		DrawGrid(10, 1.0f);
		DrawCubeWires(Vector3{ 0, 5, 0 }, 10, 10, 10, GOLD);
		
		EndMode3D();

		//=== DRAW UI ===
		//DrawRectangle(10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
		//DrawRectangleLines(10, 10, 320, 133, BLUE);
		// DrawText("Free camera default controls:", 20, 20, 10, BLACK);
		// DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
		// DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
		// DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
		// DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
		// DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);

		//DRAW FPS
		DrawFPS(0, 0);
		//
		
		EndDrawing();
		//----------------------------------------------------------------------------------

		//UPDATE DELTA TIME	
		time = time + dt;
		// ==========================
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
}
