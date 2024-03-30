//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------

#include <time.h>

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CSimpleSprite* pigeonMeme;
CSimpleSprite *testSprite;
CSimpleSprite* background;
CSimpleSprite* carWash;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
};
//------------------------------------------------------------------------

bool isOnWelcome;
bool isInGame;
bool isEnded;

CGameManager gameManager;

int count = 0;

time_t start;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	isOnWelcome = true;
	isInGame = false;
	isEnded = false;

	// do not init before hitting start.

	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite = App::CreateSprite(".\\TestData\\pigeon_item.bmp", 1, 1);
	testSprite->SetPosition(200.0f, 500.0f);
	float speed = 1.0f / 15.0f;
	testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	testSprite->SetScale(0.1f);
	//------------------------------------------------------------------------

	pigeonMeme = App::CreateSprite(".\\TestData\\Pigeon.bmp", 1, 1);
	pigeonMeme->SetPosition(500.0f, 500.0f);
	pigeonMeme->SetScale(0.5f);

	if (isOnWelcome) return;  // does not init game until hitting start

	//gameManager = CGameManager();

	//time(&start);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	if (isOnWelcome) {
		if (App::IsKeyPressed(VK_SPACE)) {  // when entering into main game...
			isOnWelcome = false;
			isInGame = true;

			gameManager = CGameManager();
			time(&start);

			delete pigeonMeme;

			background = App::CreateSprite(".\\TestData\\skyblue.bmp", 1, 1);
			background->SetPosition(0.0f, 0.0f);
			background->SetScale(5.0f);

			carWash = App::CreateSprite(".\\TestData\\car_wash.bmp", 1, 1);
			carWash->SetPosition(150.0f, 175.0f);
			carWash->SetScale(1.0f);
		}
		return;
	}
	// in main game.
	time_t currTime;
	time(&currTime);
	double seconds_since_start = difftime(currTime, start);
	if (seconds_since_start == 60.0) {
		isInGame = false;
		isEnded = true;
		return;
	}

	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite->Update(deltaTime);
	//------------------------------------------------------------------------
	// Sample Sound.
	//------------------------------------------------------------------------
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		App::PlaySound(".\\TestData\\Test.wav");
	}
	// ====== CUSTOM ======

	// player hit lmb to shoot poop!

	float sx, sy, mx, my;
	testSprite->GetPosition(sx, sy);
	App::GetMousePos(mx, my);

	sy -= 50.0f;

	gameManager.PoopUpdate(sx, sy, mx, my, App::IsKeyPressed(VK_LBUTTON), App::IsKeyPressed(VK_RBUTTON));

	gameManager.SpawnVehicle();
	gameManager.UpdateVehicles();
	// ====== ENDCUS ======
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	

	if (isOnWelcome) {
		//App::Print(400, 425, "White car, black poop.");
		//App::Print(400, 375, "Black car, white poop.");
		App::Print(50, 750, "image source: https://www.reddit.com/media?url=https%3A%2F%2Fi.redd.it%2Fi-better-get-a-polka-dot-car-then-v0-hu7pfbsosip81.jpg%3Fs%3Deb9c317e8ec313ad278e0eca108f2de9b0d2a29e.",
			1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_10);
		pigeonMeme->Draw();
		App::Print(425, 225, "Pooping Pigeon", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24);
		App::Print(412, 150, "LMB to poop WHITE", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
		App::Print(412, 125, "RMB to poop BLACK", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
		App::Print(325, 100, "Aim at and poop on the cars before they pass the car wash!", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_12);
		App::Print(375, 75, "Poop on as many cars as possible in 1 minute!", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_12);
		App::Print(400, 50, "Press SPACE to continue.");
		return;
	}
	if (isEnded) {
		char s[50];
		sprintf(s, "Score: %i", (int)gameManager.GetScore());
		App::Print(400, 400, s);
		App::Print(400, 150, "Press ESC to exit.");
		return;
	}

	background->Draw();
	// Ground at y = 100
	//App::Print(100, 110, "Ground");
	App::DrawLine(0, 100, 1000, 100, 1, 1, 1);
	carWash->Draw();
	//------------------------------------------------------------------------
	// Example Sprite Code....
	testSprite->Draw();
	//------------------------------------------------------------------------


	//CSimpleSprite* pSprite = App::CreateSprite(".\\TestData\\black_poop.bmp", 1, 1);
	//pSprite->SetPosition(400, 400);
	//pSprite->SetScale(0.25f);
	//pSprite->Draw();

	// draw a poop when it is shooted
	if (gameManager.isPoopShooting()) {
		gameManager.DrawPoop();
	}

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	//App::Print(100, 100, "Sample Text");

	// ====== CUSTOM ======

	// instruction display
	App::Print(600, 625, "LMB to poop WHITE");
	App::Print(600, 600, "RMB to poop BLACK");

	// score display
	char scoreDisplay[50];
	sprintf(scoreDisplay, "Score: %i", (int)gameManager.GetScore());
	App::Print(600, 575, scoreDisplay);

	// time display
	time_t currTime;
	time(&currTime);
	double secondsSinceStart = difftime(currTime, start);
	double timeLeft = 60.0 - secondsSinceStart;
	char p[50];
	sprintf(p, "Remaining time: %i\n", (int)timeLeft);
	App::Print(600, 550, p);

	// hitbox/hit line
	//App::DrawLine(gameManager.GetHitBoxX(), 100, gameManager.GetHitBoxX(), 400, 1, 1, 1);

	gameManager.DrawVehicles();

	// draw target & auxiliary line of the initial velocity's direction
	// only when left or right mouse button pressed (but not both)
	bool isAiming = ((App::IsKeyPressed(VK_LBUTTON) && !App::IsKeyPressed(VK_RBUTTON)) ||
		(!App::IsKeyPressed(VK_LBUTTON) && App::IsKeyPressed(VK_RBUTTON)));
	if (isAiming) {
		float sx, sy, mx, my;
		testSprite->GetPosition(sx, sy);
		App::GetMousePos(mx, my);
		sy -= 50;
		gameManager.DrawAuxiliaryLine(sx, sy, mx, my);
	}
	// ====== ENDCUS ======
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete testSprite;
	delete background;
	delete carWash;
	//------------------------------------------------------------------------
}