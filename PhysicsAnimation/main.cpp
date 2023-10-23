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
	bool quit = false;
	bool windToggle = false;
	bool lightToggle = false;
};

int GetInput(UserInput& input, SDL_Event& windowEvent) {
	int scene = -1;
	while (SDL_PollEvent(&windowEvent)) {
		if (windowEvent.type == SDL_QUIT) input.quit = true; //Exit Game Loop
		if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)
			input.quit = true; //Exit Game Loop
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
		if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_1)
			scene = 1;
		if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_2)
			scene = 2;
		if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_w)
			input.windToggle = true;
		if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_l)
			input.lightToggle = true;
	}

	return scene;
}

void MoveCamera(const UserInput& input, Camera& camera, float frameTime) {
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
}

int windSpeed = 2;
int lightDir = 3;
void HandleToggleables(UserInput& input, Scene& scene, Renderer& renderer) {
	if (input.windToggle) {
		if (windSpeed == 3) {
			scene.SetWind(Vec3F(0, 0, 0));
			std::cout << "Drag: Off\n";
			Rope::DragIsOn = false;
			windSpeed = -1;
		}
		else if (windSpeed == -1) {
			Rope::DragIsOn = true;
			std::cout << "Wind: Off, Drag: On\n";
			scene.SetWind(Vec3F(0, 0, 0));
			windSpeed++;
		}
		else  if (windSpeed == 0) {
			scene.SetWind(Vec3F(3, 0, -3));
			std::cout << "Wind: Low\n";
			windSpeed++;
		}
		else if (windSpeed == 1) {
			scene.SetWind(Vec3F(6, 0, -6));
			std::cout << "Wind: Med\n";
			windSpeed++;
		}
		else if (windSpeed == 2) {
			scene.SetWind(Vec3F(10, 0, -10));
			std::cout << "Wind: High\n";
			windSpeed++;
		}

		input.windToggle = false;
	}

	if (input.lightToggle) {
		if (lightDir == 3) {
			renderer.SetLightDirection(Vec3F(1, 0, 0));
			std::cout << "Light: Front\n";
			lightDir = 0;
		}
		else if (lightDir == 0) {
			renderer.SetLightDirection(Vec3F(0, -1, 0));
			std::cout << "Light: Top\n";
			lightDir++;
		}
		else if (lightDir == 1) {
			renderer.SetLightDirection(Vec3F(-1, 0, 0));
			std::cout << "Light: Back\n";
			lightDir++;
		}
		else if (lightDir == 2) {
			renderer.SetLightDirection(Vec3F(0, 1, 0));
			std::cout << "Light: Bot\n";
			lightDir++;
		}

		input.lightToggle = false;
	}
}

void GiveUserInstructions() {
	std::cout << "Press 1, 2 to toggle between scenes\n";
	std::cout << "Press w to toggle wind\n";
	std::cout << "Press l to toggle the light source location\n";
}

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
	camera.SetAspect(screenDetails.width, screenDetails.height);

	// Main Loop
	SDL_Event windowEvent;
	bool quit = false;

	Scene scene = Scene(1,10,Vec3F(0,-10,0), Vec3F(10,0,-10), &renderer);
	scene.LoadScene(1, camera);

	auto lastFrameTime = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> targetFrameTime(0.01);
	
	UserInput input;
	GiveUserInstructions();
	input.windToggle = true;
	input.lightToggle = true;
	HandleToggleables(input, scene, renderer); // Done so that it outputs the scene state to console

	while (!quit) {
		// Keyboard events
		int newSceneId = GetInput(input, windowEvent);
		HandleToggleables(input, scene, renderer);

		if (newSceneId != -1) {
			scene.LoadScene(newSceneId, camera);
			camera.SetAspect(screenDetails.width, screenDetails.height);
		}

		auto thisFrameTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float>dt = thisFrameTime - lastFrameTime;

		renderer.StartFrame();
		renderer.SetCamera(camera);
		float frameTime = dt.count();
		
		MoveCamera(input, camera, frameTime);
		renderer.SetCamera(camera);

		scene.update(frameTime);
		scene.render();

		renderer.FinalizeFrame();

		lastFrameTime = thisFrameTime;

		SDL_GL_SwapWindow(window); //Double buffering
	}

	return 0;
}