#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>

const int screenWidth = 500;
const int screenHeight = 600;

struct Player {
	Rectangle rect;
	float velocity = 250.0f;
	int score = 0;
	float w = 75.0f;
	float h = 10.0f;
};

struct Ball {
	Vector2 pos;
	Vector2 accel {1.0f, 1.0f};
	float velocity;
	float radius = 5.0f;
};

struct Brick {
	Rectangle rect;
	Color color;
	float w = 50.0f;
	float h = 20.0f;
};

Texture2D texBackground;
Player player;
Ball ball;

std::vector<Brick> bricks;

void GameStartup() {

	// loading background img into vram
	Image imgBackground = LoadImage("assets/background.png");
	texBackground = LoadTextureFromImage(imgBackground);
	UnloadImage(imgBackground);

	// initializing player paddle
	player.rect = Rectangle{ 250.0f, 540.0f, player.w, player.h};
	player.score = 0;

	//initialize ball data
	ball.pos = Vector2{300, 300};
	ball.velocity = 300.0f;

	Brick newBrick;
	for (int i = 0; i < 8 ; i++) {
		for (int j = 0; j < 8; j++) {
			newBrick.rect = Rectangle {
				float ( 40 + (i * 55)),
				float (50 + (j * 26)),
				newBrick.w,
				newBrick.h
			};
			newBrick.color = RED;
			bricks.push_back(newBrick);
		}
	}
}

void GameUpdate() {
	float frameTime = GetFrameTime();

	// update player rect with arrow keys
	if (IsKeyDown(KEY_LEFT)) {
		player.rect.x -= player.velocity * frameTime;
	}
	if (IsKeyDown(KEY_RIGHT)) {
		player.rect.x += player.velocity * frameTime;
	}

	// check for rect wall collision
	if (player.rect.x < 0) {
		player.rect.x = 0;
	}

	if (player.rect.x > (screenWidth - player.rect.width)) {
		player.rect.x = (screenWidth - player.rect.width);
	}

	// update ball position
	ball.pos.x += (ball.velocity * ball.accel.x) * frameTime;
	ball.pos.y += (ball.velocity * ball.accel.y) *frameTime;

	// check for ball wall collision 
	if (ball.pos.x > screenWidth || ball.pos.x < 5) {
		ball.accel.x *= -1;
	}
	if (ball.pos.y > screenHeight || ball.pos.y < 5) {
		ball.accel.y *= -1;
	}

	// check collision between BALL and PLAYER
	if (CheckCollisionCircleRec(ball.pos, ball.radius, player.rect)){
		ball.accel.x *= -1;
		ball.accel.y *= -1;
	}

	// check collision between ball and brick
	Brick brick;
	for (int i = 0; i < bricks.size(); i++) {
		brick = bricks [i];
		if (CheckCollisionCircleRec(ball.pos, ball.radius, brick.rect)) {
			ball.accel.y *= -1;
			bricks.erase(bricks.begin() + i);
			player.score += 10;

		}
	}
}

void GameRender() {
	// Draw from furthest to closest
	
	// Draw background
	DrawTexture(texBackground, 0, 0, RAYWHITE);

	// Draw Bricks
	Brick brick;
	for ( int i = 0; i < bricks.size(); i++) {
		brick = bricks[i],
		DrawRectangle (brick.rect.x,
			       brick.rect.y,
			       brick.rect.width,
			       brick.rect.height, 
			       brick.color);
	}

	// Draw paddle
	DrawRectangle(player.rect.x, player.rect.y, player.rect.width, player.rect.height, RAYWHITE);
	// Draw BALL
	DrawCircle(ball.pos.x, ball.pos.y, ball.radius, ORANGE);

	// Draw score UI
	std::string sScore = std::to_string(player.score);
	std::string temp1 = "SCORE: ";
	std::string temp2 = temp1 + sScore;
	char const* text = temp2.c_str();
	DrawText(text, 10, 10, 30, YELLOW);
}


void GameShutdown() {

}

int main () {
	std::cout <<"hello bro\n";
	InitWindow(screenWidth, screenHeight, "Raylib Breakout");

	SetTargetFPS(60);

	GameStartup();

	while (!WindowShouldClose()) {

		GameUpdate();

		BeginDrawing();
		ClearBackground(RED);

		GameRender();

		EndDrawing();
	}

	GameShutdown();
	CloseWindow();

	return 0;
}

