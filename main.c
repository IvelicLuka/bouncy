#include <stdio.h>
#include "raylib.h"

#define BACKGROUND_COLOR CLITERAL(Color){48, 48, 48, 255}
#define WIDTH 800
#define HEIGHT 600
#define BAR_HEIGHT 25
#define BAR_WIDTH  150
#define BAR_OFFSET 50
#define BALL_RADIUS 5
#define BALL_SPEED 15
#define ROWS 3
#define COLS 5
#define TAR_OFFSET 20
#define FRACTION_TARS 3

Color Colors[] = {
GRAY,
YELLOW,   
ORANGE,   
PINK,     
RED,    
LIME,     
SKYBLUE,  
PURPLE,   
BROWN,    
MAGENTA,  
};

typedef struct {
	Rectangle r;
	int n;
} target;
int main(void) {
	InitWindow(WIDTH, HEIGHT, "jooo");
	SetTargetFPS(60);

	float x;
	Rectangle Bar = { 0 };
	Bar.y = HEIGHT - BAR_OFFSET - BAR_HEIGHT;
	Bar.width = BAR_WIDTH;
	Bar.height = BAR_HEIGHT;

	Vector2 ballPos = { WIDTH/2, HEIGHT/2 };
	Vector2 ballVel = { 0, BALL_SPEED };
	
	target tar[ROWS][COLS] = { 0 };

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			Rectangle tarBar = CLITERAL(Rectangle){
				.x = j*(WIDTH-(COLS+1.0f)*TAR_OFFSET)/COLS + (j+1.0f)*TAR_OFFSET,
				.y = (i*(HEIGHT-(ROWS+1.0f)*TAR_OFFSET*3)/ROWS + (i+1.0f)*TAR_OFFSET*3) / FRACTION_TARS,
				.width = (WIDTH-(COLS+1.0f)*TAR_OFFSET)/COLS,
				.height = ((HEIGHT-(ROWS+1.0f)*TAR_OFFSET*3)/ROWS) / FRACTION_TARS};
			tar[i][j].n = 5;
			tar[i][j].r = tarBar;
		}
	}

	while(!WindowShouldClose()) {
		x = GetMouseX();
		if (x < BAR_WIDTH/2) x = BAR_WIDTH/2;
		if (x > WIDTH - BAR_WIDTH/2) x = WIDTH - BAR_WIDTH/2;
		Bar.x = x - BAR_WIDTH/2;
		
		ballPos.x += ballVel.x;
		ballPos.y += ballVel.y;
		if (ballPos.x < BALL_RADIUS || ballPos.x > WIDTH - BALL_RADIUS) ballVel.x = -ballVel.x;
		if (ballPos.y < BALL_RADIUS || ballPos.y > HEIGHT - BALL_RADIUS) ballVel.y = -ballVel.y;
		if (CheckCollisionCircleRec(ballPos, BALL_RADIUS, Bar)) {
			ballVel.y = -(HEIGHT - (BAR_OFFSET + BAR_HEIGHT/2) - ballPos.y)/BAR_HEIGHT*BALL_SPEED;
			ballVel.x = -(x - ballPos.x)/BAR_WIDTH*BALL_SPEED;
		}

		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
			if (CheckCollisionCircleRec(ballPos, BALL_RADIUS, tar[i][j].r) && tar[i][j].n > 0) {
					ballVel.y = -((tar[i][j].r.y + tar[i][j].r.height/2 - ballPos.y))/BAR_HEIGHT*BALL_SPEED;
					ballVel.x = -(tar[i][j].r.x + tar[i][j].r.width/2 - ballPos.x)/BAR_WIDTH*BALL_SPEED;
					tar[i][j].n -= 1;
					//ballVel.y = -ballVel.y;
				}
			}
		}

		BeginDrawing();
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLS; j++) {
					if(tar[i][j].n > 0) {
						DrawRectangleRec(tar[i][j].r, Colors[tar[i][j].n]);
					}
				}
			}
			DrawCircle(ballPos.x, ballPos.y, BALL_RADIUS, YELLOW);
			DrawRectangleRec(Bar, WHITE);
			ClearBackground(BACKGROUND_COLOR);
		EndDrawing();
	}
}
