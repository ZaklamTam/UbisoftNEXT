#include "stdafx.h"
#include "GameManager.h"
#include "Vehicle.h"

CGameManager::CGameManager() {
	PoopVParallel = 0;
	PoopVVertical = 0;
	currPoopColor = 0;
	currScore = 0;
	pSprite = App::CreateSprite(".\\TestData\\poop_black.bmp", 1, 1);
	pSprite->SetPosition(px, py);
	pSprite->SetScale(0.1f);
}

bool CGameManager::isPoopShooting() { return shooting; }

void CGameManager::GetPoopPos(float& x, float& y) {
	x = px;
	y = py;
}

void CGameManager::SpawnVehicle() {
	// randomly spawn vehicle with random color... 
	if (currSpawnCD > 0) currSpawnCD--;
	else if (vehicleQ.size() < capacity) {// only add if queue not full
		// 75% chance to spawn new vehicle.
		float p = static_cast<float> (std::rand()) / RAND_MAX;
		char sp[50];
		sprintf(sp, "p = %f\n", p);
		OutputDebugStringA(sp);
		if (p <= 0.75f) {
			p = static_cast<float> (std::rand()) / RAND_MAX;  // possibility for color.
			bool color = (p <= 0.5) ? 0 : 1;

			CVehicle v = CVehicle(color, 900, 100);
			vehicleQ.push_back(v);
		}
		currSpawnCD = vehicleMinDistance;
	}
}

void CGameManager::UpdateVehicles() {
	// move vehicles left
	for (int i = 0; i < vehicleQ.size(); i++) {
		auto v = std::next(vehicleQ.begin(), i);
		float vx, vy;
		v->GetPos(vx, vy);
		// TODO: replace this 1.0f with "SPEED"?
		vx -= 1.0f;
		v->SetPos(vx, vy);
	}
	// pop all the vehicles outside of scene at the queue front.
	while (!vehicleQ.empty()) {
		CVehicle v = vehicleQ.front();
		//vehicleQ.pop_front();
		float vx, vy;
		v.GetPos(vx, vy);
		if (vx < 0) {
			vehicleQ.pop_front();
		} else {
			// break on the first vehicle in scene.
			break;
		}
	}
}

void CGameManager::DrawVehicles() {
	for (CVehicle v: vehicleQ) {
		// hide the ones pooped before hitbox
		// -- they went to car wash...
		if (v.isPooped() && v.hasPassedHitbox(hitboxX)) continue;
		v.DrawVehicle();
	}
}

void CGameManager::DrawPoop() {
	/***
	App::DrawLine(px - 5, py - 5, px + 5, py + 5, 0, 0, 0);
	App::DrawLine(px - 5, py + 5, px + 5, py - 5, 0, 0, 0);
	***/
	// TODO: WHY CAN'T I DRAW POOP SPRITE?
	///***
	free(pSprite);
	if (currPoopColor) pSprite = App::CreateSprite(".\\TestData\\black_poop.bmp", 1, 1);
	else pSprite = App::CreateSprite(".\\TestData\\white_poop.bmp", 1, 1);
	pSprite->SetPosition(px, py);
	pSprite->SetScale(0.1f);
	pSprite->Draw();
	//***/
}

void CGameManager::DrawAuxiliaryLine(float sx, float sy, float mx, float my) {
	App::DrawLine(sx, sy, mx, my, 1, 0, 0);
	// draw target, position when hit the ground.
	// v0 start velocity
	// euclidean distance between char and mouse pos
	float tx, ty;
	GetTargetPos(sx, sy, mx, my, tx, ty);
	App::DrawLine(tx - 5, ty - 5, tx + 5, ty + 5, 1, 0, 0);
	App::DrawLine(tx - 5, ty + 5, tx + 5, ty - 5, 1, 0, 0);
}

void CGameManager::PoopUpdate(float sx, float sy, float mx, float my, bool isLMBPressed, bool isRMBPressed) {
	// LMB for White 0; RMB for Black 1
	if ((isLMBPressed) && (!isRMBPressed)) currPoopColor = 0;
	else if ((!isLMBPressed) && (isRMBPressed)) currPoopColor = 1;
	if (!shooting) {  // if the poop is not on the fly
		// 2 cases
		if ((isLMBPressed || isRMBPressed) && !(isLMBPressed && isRMBPressed)) {  // aiming (if only 1 button is pressed)
			OutputDebugStringA("holding mouse\n");
			poopReady = true;
			PoopVParallel = (mx - sx) * 0.25;
			PoopVVertical = (my - sy) * 0.25;
		} else if (poopReady) {  // the moment before pooping
			OutputDebugStringA("mouse released\n");
			shooting = true;
			poopReady = false;
			// set the sprite (pigeon) pos as starting point.
			px = sx;
			py = sy;
		}
	} else {  // now poop is on the fly
		// TODO: replace 100 with constant GROUND
		if (py >= 100) {
			OutputDebugStringA("flying\n");
			px += 0.5 * (PoopVParallel);
			py += 0.5 * (PoopVVertical);
			PoopVVertical -= 0.5 * (G);
			// check if the poop is hitting any vehicle.
			// get the container of pooped vehicle.
			std::deque<CVehicle>::iterator iv;
			//float vx, vy;
			//iv->GetPos(vx, vy);
			//vx = 500;
			if (isPoopHittingVehicle(iv)) {
				shooting = false;
				iv->Poop(currPoopColor);
			}
		} else {
			OutputDebugStringA("stopped\n");
			shooting = false;
		}
	}
}

void CGameManager::GetTargetPos(float sx, float sy, float mx, float my, float& tx, float& ty) {
	ty = 110.0f;
	float slowdownFactor = 0.25f;
	// get velocities on x and y direction.
	float vParallel = mx - sx;
	float vVertical = my - sy;
	//float v = sqrt(pow(vParallel, 2) + pow(vVertical, 2));
	vParallel *= slowdownFactor;
	vVertical *= slowdownFactor;
	//v *= 0.25;
	// Let x be the distance travelled on x-axis, y be the distance on y-axis, then for time t:
	// x = vp*t, y = vv*t - (1/2)*G*t^2 => because the starting point is (200, 400)
	// => y = (vv/vp)*x - (G/(2*vp^2))*x^2
	// want to get it when y = 100 - 500 = -400, since the starting point is (200, 400)  (TODO!)
	// solve -(G/(2*vp^2))*x^2 + (vv/vp)*x + 400 = 0
	// also pad the root x with 200.
	float a = 0;
	float b = 0;
	float c = 0;
	float delta = 0;
	float groundX = 200;
	if (vParallel) {
		a = -(G / (2 * pow(vParallel, 2)));
		b = vVertical / vParallel;
		c = 400;
		delta = pow(b, 2) - 4 * a * c;
		// Take x according to the shooting direction.
		if (vParallel > 0) groundX = (-b - sqrt(delta)) / (2 * a) + 200;
		else groundX = (-b + sqrt(delta)) / (2 * a) + 200;
	}
	tx = groundX;
}

bool CGameManager::isPoopHittingVehicle(std::deque<CVehicle>::iterator& ivehicle) {
	for (int i = 0; i < vehicleQ.size(); i++) {
		auto v = std::next(vehicleQ.begin(), i);
		if (v->Hit(px, py)) {
			ivehicle = v;
			// only vehicle that's not passed can be pooped on
			return !v->hasPassedHitbox(hitboxX);
			// since there are distance between vehicles
			// a poop hit no more than 1 vehicle at the same time.
		}
	}
	return false;
}

float CGameManager::GetHitBoxX() { return hitboxX;  }

float CGameManager::GetScore() { 
	// update score
	for (int i = 0; i < vehicleQ.size(); i++) {
		auto iv = std::next(vehicleQ.begin(), i);
		if (iv->isPooped() && iv->hasPassedHitbox(hitboxX) && !iv->isScored()) {
			currScore += 100.0f;
			iv->Score();
		}
	}
	return currScore; 
}