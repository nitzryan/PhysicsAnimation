#include "glad/glad.h"
#include "sdl/SDL.h"

#include "rendering/Renderer.h"
#include "rendering/Camera.h"
#include <iostream>
#include <chrono>

#include "water/ShallowWater.h"
#include "Scene.h"

struct ScreenDetails {
	bool fullscreen;
	int width;
	int height;
};

ScreenDetails screenDetails{ false, 800, 800 };

struct UserInput {
	bool leftPressed = false;
	bool rightPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	bool lctrl = false;
};

int main(int, char**) {
	// Setup SDL
	SDL_Init(SDL_INIT_VIDEO);
	// Get Versions
	SDL_version comp;
	SDL_version linked;
	SDL_VERSION(&comp);
	SDL_VERSION(&linked);
	//Ask SDL to get a recent version of OpenGL (3.2 or greater)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	//Create a window (offsetx, offsety, width, height, flags)
	SDL_Window* window = SDL_CreateWindow("CSCI5611 Proj2", 100, 100,
		screenDetails.width, screenDetails.height, SDL_WINDOW_OPENGL);
	if (!window) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1; //Exit as SDL failed
	}
	float aspect = screenDetails.width / (float)screenDetails.height; //aspect ratio needs update on resize
	SDL_GLContext context = SDL_GL_CreateContext(window); //Bind OpenGL to the window

	// Initialize OpenGL
	if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
	}
	else {
		printf("ERROR: Failed to initialize OpenGL context.\n");
		return -1;
	}


	Renderer renderer = Renderer();
	Camera camera(1, 1, Pos3F(1, 5, 6), Vec3F(0, -1, -2), Vec3F(0, 2, -1));
	//Camera camera(1, 1, Pos3F(1, 10, 1), Vec3F(0, -1, 0), Vec3F(0, 0, 1));
	camera.SetAspect(screenDetails.width, screenDetails.height);

	ShallowWater water = ShallowWater(Pos3F(0, 0, 0), Pos3F(2, 2, 2), 25, 25, 0.1);

	// Main Loop
	SDL_Event windowEvent;
	bool quit = false;

	//TestRenderable testRenderable = TestRenderable();
	// test code to see if cloth works
	std::vector<Rope> ropes;
	for (int i = 0; i < 80; i++) {
		Rope rope = Rope(25, .02, Pos3F(i*.01,0,i*.01), Vec3F::Cross(Vec3F(.5,0,.5),Vec3F(0,-1,0)).GetNormalized());
		ropes.push_back(rope);
	}


	Scene scene = Scene(1,10,Vec3F(0,-10,0), Vec3F(10,0,-10), &renderer);
	Cloth cloth = Cloth(ropes);
	SphereRenderable sphere = SphereRenderable(Pos3F(.25, -0.4, .25), .25);
	scene.add_cloth(cloth);
	scene.add_sphere(sphere);

	auto lastFrameTime = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> targetFrameTime(0.01);
	
	UserInput input;
	float time_accum = 0;
	while (!quit) {
		// Keyboard events
		while (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) quit = true; //Exit Game Loop
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)
				quit = true; //Exit Game Loop
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) {
				screenDetails.fullscreen = !screenDetails.fullscreen;
				SDL_SetWindowFullscreen(window, screenDetails.fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_LEFT)
				input.leftPressed = false;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_RIGHT)
				input.rightPressed = false;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_DOWN)
				input.downPressed = false;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_UP)
				input.upPressed = false;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_LCTRL)
				input.lctrl = false;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT)
				input.leftPressed = true;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT)
				input.rightPressed = true;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN)
				input.downPressed = true;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP)
				input.upPressed = true;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LCTRL)
				input.lctrl = true;
		}

		auto thisFrameTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float>dt = thisFrameTime - lastFrameTime;

		renderer.StartFrame();
		renderer.SetCamera(camera);
		float frameTime = dt.count();
		
		if (input.lctrl) {
			float w = 0.6;
			if (input.rightPressed) {
				camera.RotateY(w, frameTime);
			}
			if (input.leftPressed) {
				camera.RotateY(-w, frameTime);
			}
			if (input.upPressed) {
				camera.RotateX(w, frameTime);
			}
			if (input.downPressed) {
				camera.RotateX(-w, frameTime);
			}
		}
		else {
			if (input.rightPressed) {
				camera.StepX(frameTime);
			}
			if (input.leftPressed) {
				camera.StepX(-frameTime);
			}
			if (input.upPressed) {
				camera.StepZ(frameTime);
			}
			if (input.downPressed) {
				camera.StepZ(-frameTime);
			}
		}
		
		time_accum += frameTime;

		water.Update(frameTime);
		renderer.Render(water);
		scene.update(frameTime);
		scene.render();

		renderer.FinalizeFrame();

		lastFrameTime = thisFrameTime;

		SDL_GL_SwapWindow(window); //Double buffering

	}

	return 0;
}