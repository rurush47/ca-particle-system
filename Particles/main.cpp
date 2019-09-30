#include <iostream>
#include <vector>
#include "Particle.h"
#include "Geometry.h"
#include "raylib.h"

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
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type

	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

	SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

	SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

	//======

	//=== PARTICLE CONSTS ===
	float dt = 0.01f;  //simulation time
	float tini = 0.0f; 
	float tfinal = 9000; //final time of simulation 
	// particle inicialization
	Particle p(0.0f, 5.0f, 0.0f); //initial position of the particle

	p.setLifetime(900.0f);
	std::cout << "Lifetime =" << p.getLifetime() << std::endl;
	p.setBouncing(0.85f);
	p.addForce(0, -9.8f, 0);
	p.setVelocity(10, 0, 12);
	//p.setFixed(true);

	// define a floor plane for collision
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

	glm::vec3 sphereCenter(0, 1, 0);
	Sphere sphere(sphereCenter, 2);
	
	// simulation loop
	int count = 0;
	float disact, disant;
	float time = tini;

	// Main game loop
	while (!WindowShouldClose())        // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		UpdateCamera(&camera);          // Update camera

		if (IsKeyDown('Z')) camera.target = *new Vector3{ 0.0f, 0.0f, 0.0f };
		
		if (IsKeyDown('X'))
		{
			p.addForce(0.2, 0, 0);
		}
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		//PARTICLE CODE
		if (p.getLifetime() > 0) {
			p.updateParticle(dt, Particle::UpdateMethod::EulerOrig);
			std::cout << "position = " << p.getCurrentPosition().x << "  " << p.getCurrentPosition().y
				<< "  " << p.getCurrentPosition().z << "  time = " << time << std::endl;
			//Check for floor collisions
			
			p.setLifetime(p.getLifetime() - dt);

			for (Plane plane : planes)
			{
				disact = plane.distPoint2Plane(p.getCurrentPosition());
				if (disact < 0.0f) 
				{
					auto [newPos, newVelocity] = plane.getCollisionProducts(p.getCurrentPosition(), p.getVelocity());

					p.setPosition(newPos);
					p.setVelocity(newVelocity);
					std::cout << "Bounce = " << count++ << std::endl;
					//disact = -disact; //
					//system("PAUSE");
					time = time + dt; //increase time counter
				}
			}

			if(sphere.isInside(p.getCurrentPosition()))
			{
				auto intersectionPoint = sphere.getIntersectionPoint(p.getPreviousPosition(), p.getCurrentPosition());
				auto [newPos, newVelocity] = sphere.getCollisionProducts(p.getCurrentPosition(), p.getVelocity(), intersectionPoint);

				p.setPosition(newPos);
				p.setVelocity(newVelocity);

				time = time + dt;
			}
			//disant = disact;
		}
		// ==========================

		//=== OBJECTS RENDERING
		DrawSphere(*p.getCurrentPositionRaylib(), 0.1, GREEN);
		//================
		sphere.render();
		//================
		
		DrawGrid(10, 1.0f);

		EndMode3D();

		DrawRectangle(10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
		DrawRectangleLines(10, 10, 320, 133, BLUE);

		DrawText("Free camera default controls:", 20, 20, 10, BLACK);
		DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
		DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
		DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
		DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
		DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	
}
