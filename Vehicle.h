#pragma once
//#include "stdafx.h"
#include "App/SimpleSprite.h"

class CVehicle {
private:
	bool color;  // 0 for black, 1 for white
	float posX;
	float posY;
	bool pooped;
	float collision;
	bool scored;
	CSimpleSprite* vSprite;

public:
	// default
	CVehicle();
	// constructor for a vehicle
	CVehicle(bool c, float x, float y);

	// getters. color cannot be reset
	bool GetColor();
	void GetPos(float& x, float& y);

	// setters. 
	void SetPos(float x, float y);

	// if the vehicle is hit by a poop nearby...
	bool Hit(float px, float py);

	// a vehicle can be pooped on.
	// if the color does not match, vehicle will dissolve
	// ("like dissolves like"?)
	// the driver will be alerted and take action immediately
	void Poop(bool c);
	bool isPooped();

	// return true if the vehicle survived through the poop boombing.
	bool hasPassedHitbox(float hitBoxX);

	// set score to true
	void Score();
	bool isScored();
	
	void DrawVehicle();
};