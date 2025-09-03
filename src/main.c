#include "raylib.h"
#include "../include/resource_dir.h"

int main() {
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);

	InitWindow(1280, 800, "Raylib Project");
	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();

		BeginDrawing();
		ClearBackground(BLACK);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

