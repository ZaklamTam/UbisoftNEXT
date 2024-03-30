#pragma once
#include "stdafx.h"
#include "app\app.h"
#include <list>
#include <queue>
#include "Vehicle.h"

class CGameManager {
private:
	std::deque<CVehicle> vehicleQ;
	// capacity of vehicleQ
	int capacity = 9;
	int vehicleMinDistance = 100;
	int currSpawnCD = 10;

	// Poop values.
	bool poopReady = false;
	bool shooting = false;
	float PoopVVertical, PoopVParallel;
	float px = 0;
	float py = 0;
	bool currPoopColor;

	// hit line
	float hitboxX = 200.0f;

	// curr score
	float currScore;

	// poop sprite
	CSimpleSprite* pSprite;

public:

	CGameManager();

	// try spawnning vehicle if the queue is not full
	void SpawnVehicle();

	// update vehicles position, clear the ones out of scene.
	void UpdateVehicles();

	// Display elements.
	void DrawVehicles();
	void DrawPoop();
	// draw auxiliary line and target
	void DrawAuxiliaryLine(float sx, float sy, float mx, float my);

	// TEMP
	std::deque<CVehicle> GeQ() {
		return vehicleQ;
	}

	// Poop values
	// True when poop ready to shoot (aiming)
	bool isPoopingReady();
	// True when poop is on the fly, literally.
	bool isPoopShooting();
	// get current poop velocity in x and y directions.
	void GetPoopVelocity(float& vx, float& vy);
	// get current poop position
	void GetPoopPos(float& x, float& y);
	// setter for velocity
	void SetPoopVelocity(float vx, float vy);
	// setter for position
	void SetPoopPos(float x, float y);
	// update poop location
	// take sprite location (sx, sy); mouse location (mx, my); bool true if aiming (LMB is hold)
	void PoopUpdate(float sx, float sy, float mx, float my, bool isLMBPressed, bool isRMBPressed);

	// Calculate the target position for display when aiming (LMB is hold).
	void GetTargetPos(float sx, float sy, float mx, float my, float& tx, float& ty);

	// check if the current poop is hitting any of the vehicles
	bool isPoopHittingVehicle(std::deque<CVehicle>::iterator& vehicle);

	float GetHitBoxX();
	// get updated score.
	float GetScore();
};