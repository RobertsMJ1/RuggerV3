#include "Player.h"


Player::Player(void) : GameObject()
{
}


Player::~Player(void)
{
	box = 0;
}

void Player::init(Box* b, vector<Bullet*> theBullets, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	box = b;
	Player::bullets = theBullets;
	radius = r;
	radius *= 1.01f;
	position = pos;
	velocity = vel;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
	width = s;
	height = s;
	depth = s;
	targetVector = Vector3(1, 0, 0);
	timeSinceLastShot = 500;
	timeCharged = 0;
	charging = false;
	rage = 1;

}

void Player::draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP)
{
	if (!active)
		return;
	Matrix mWVP = world* (*mVP);
	mfxWVPVar->SetMatrix((float*)&mWVP);
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        box->draw();
		for (int i = 0; i < bullets.size(); i++) 
			if(bullets[i]->getActiveState())bullets[i]->draw(mfxWVPVar, mTech, mVP);
    }

}

void Player::update(float dt)
{
	position += velocity*dt;
	Identity(&world);
	D3DXMatrixScaling(&mScale, width, height, depth);
	D3DXMatrixTranslation(&mTranslate, position.x, position.y, position.z);
	D3DXMatrixMultiply(&world, &mScale, &mTranslate);
	//Translate(&world, position.x, position.y, position.z);
	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->update(dt);
	
	timeSinceLastShot+=dt;
	if (charging) 
		timeCharged+=dt;
}

void Player::charge() {
	charging = true;
}

void Player::shoot(int direction)
{
	if (timeCharged > 1.5) {
		timeCharged = 0;
		charging = false;
		rage = 1;
	}
	if (charging)
		rage = 2;
	if (timeSinceLastShot > 0.3/rage)
		timeSinceLastShot = 0;
	else return;
	//If the player's got an active bullet on the level, he doesn't get to shoot
	int index = -1;
	for (int i = 0; i < bullets.size(); i++) {
		if (!bullets[i]->getActiveState()) { //If the bullet at index i is not in use
			index = i;
			i = bullets.size();
		}
	}
	if (index == -1) return;

	bullets[index]->setPosition(position);
	bullets[index]->setSpeed(bulletNS::SPEED);
	
	Vector3 nDir;
	float multiplier = 1;
	if (direction == UP) {
		nDir = Vector3(1,0,0);
		if (this->getVelocity().z > 0) nDir.z = 1;
		if (this->getVelocity().z < 0) nDir.z = -1;
		nDir.x *= bulletNS::SPEED*rage;
		nDir.z *= this->getSpeed()*multiplier;
	}
	else if (direction == DOWN) {
		nDir = Vector3(-1, 0, 0);
		if (this->getVelocity().z > 0) nDir.z = 1;
		if (this->getVelocity().z < 0) nDir.z = -1;
		nDir.x *= bulletNS::SPEED*rage;
		nDir.z *= this->getSpeed()*multiplier;
	}
	else if (direction == LEFT) {
		nDir = Vector3(0,0,1);
		if (this->getVelocity().x > 0) nDir.x = 1;
		if (this->getVelocity().x < 0) nDir.x = -1;
		nDir.z *= bulletNS::SPEED*rage;
		nDir.x *= this->getSpeed()*multiplier;
	}
	else if (direction == RIGHT) {
		nDir = Vector3(0,0,-1);
		if (this->getVelocity().x > 0) nDir.x = 1;
		if (this->getVelocity().x < 0) nDir.x = -1;
		nDir.z *= bulletNS::SPEED*rage;
		nDir.x *= this->getSpeed()*multiplier;
	}

	bullets[index]->setVelocity(nDir);
	bullets[index]->setActive();
}

void Player::rotateTargeting(int s)
{
	//0 for rotating ccw, 1 for cw
	Matrix rotate;
	Identity(&rotate);
	D3DXVECTOR4 tV(0, 0, 0, 0);
	tV.x = targetVector.x;
	tV.y = targetVector.y;
	tV.z = targetVector.z;
	switch(s)
	{
	case 0:
		D3DXMatrixRotationY(&rotate, ToRadian(-0.25f));
		D3DXVec4Transform(&tV, &tV, &rotate);
		
		break;
	case 1:
		D3DXMatrixRotationY(&rotate, ToRadian(0.25f));
		D3DXVec4Transform(&tV, &tV, &rotate);
		break;
	}
	targetVector.x = tV.x;
	targetVector.y = tV.y;
	targetVector.z = tV.z;
}