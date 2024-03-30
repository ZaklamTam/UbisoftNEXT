#include "stdafx.h"
#include "../Vehicle.h"

CVehicle::CVehicle() {}
CVehicle::CVehicle(bool c, float x, float y): color(c), posX(x), posY(y), collision(37.0f), pooped(false), scored(false) {
	if (color) { // black
		vSprite = App::CreateSprite(".\\TestData\\black.bmp", 1, 1);
	} else {
		vSprite = App::CreateSprite(".\\TestData\\white.bmp", 1, 1);
	}
	vSprite->SetPosition(posX, posY);
	vSprite->SetScale(0.05f);
}

bool CVehicle::GetColor() { return color; }

void CVehicle::GetPos(float& x, float& y) {
	x = posX;
	y = posY;
}

void CVehicle::SetPos(float x, float y) {
	posX = x;
	posY = y;
	vSprite->SetPosition(posX, posY);
}

bool CVehicle::Hit(float px, float py) {
	return ((posX - collision <= px) && (px <= posX + collision)) && 
		((posY - collision <= py) && (py <= posY + collision));
}

void CVehicle::Poop(bool c) {
	if (c != color) {
		pooped = true;
		if (color) {
			vSprite = App::CreateSprite(".\\TestData\\black_pooped.bmp", 1, 1);
			vSprite->SetScale(0.05f);
		} else {
			vSprite = App::CreateSprite(".\\TestData\\white_pooped.bmp", 1, 1);
			vSprite->SetScale(0.05f);
		}
	}
	//else alerted = true;
}

bool CVehicle::isPooped() { return pooped; }

bool CVehicle::hasPassedHitbox(float hitBoxX) {
	return posX <= hitBoxX;
}

void CVehicle::Score() { scored = true; }

bool CVehicle::isScored() { return scored; }

void CVehicle::DrawVehicle() { vSprite->Draw(); }