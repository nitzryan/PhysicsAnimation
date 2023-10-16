#include "glad/glad.h"
#include "sdl/SDL.h"

#include "rendering/Renderer.h"
#include "rendering/Camera.h"
#include <iostream>
#include <chrono>

#include "rendering/TestRenderable.h"
#include "rendering/TestCloth.h"

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
	SDL_Window* window = SDL_CreateWindow("CSCI5611 Proj1", 100, 100,
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
	Camera camera(1, 1, Pos3F(0.1, 0.25, -5));
	camera.SetAspect(screenDetails.width, screenDetails.height);

	// Main Loop
	SDL_Event windowEvent;
	bool quit = false;

	//TestRenderable testRenderable = TestRenderable();
	TestCloth cloth = TestCloth();

	auto lastFrameTime = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> targetFrameTime(0.01);
	
	UserInput input;
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
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT)
				input.leftPressed = true;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT)
				input.rightPressed = true;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN)
				input.downPressed = true;
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP)
				input.upPressed = true;
		}

		auto thisFrameTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float>dt = thisFrameTime - lastFrameTime;

		renderer.StartFrame();
		renderer.SetCamera(camera.GetPos(), camera.GetViewportTrans(), Vec3F(0,0,1));
		float frameTime = dt.count();
		if (input.rightPressed) {
			camera.Translate(Vec3F(-1, 0, 0), frameTime);
		}
		if (input.leftPressed) {
			camera.Translate(Vec3F(1, 0, 0), frameTime);
		}
		if (input.upPressed) {
			camera.Translate(Vec3F(0, 1, 0), frameTime);
		}
		if (input.downPressed) {
			camera.Translate(Vec3F(0, -1, 0), frameTime);
		}

		cloth.Update(frameTime);
		renderer.Render(cloth);
		renderer.FinalizeFrame();

		lastFrameTime = thisFrameTime;

		SDL_GL_SwapWindow(window); //Double buffering

	}

	return 0;
}