#include "raylib.h"
#include "config.h"

int main() {
	Config conf = (Config) {0};
	ConfigRead(&conf, "options.conf");

	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);

	InitWindow(1280, 800, "Raylib Project");
	//SetTargetFPS(60);

	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();

		BeginDrawing();
		ClearBackground(BLACK);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}

