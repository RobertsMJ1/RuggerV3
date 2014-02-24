#include "Player.h"


Player::Player(void) : GameObject()
{
}


Player::~Player(void)
{
	box = 0;
	bullet = 0;
}

void Player::init(Box* b, Bullet* bullet, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	box = b;
	Player::bullet = bullet;
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
	theta = 0;
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
		if(bullet->getActiveState())bullet->draw(mfxWVPVar, mTech, mVP);
    }
	Identity(&world);
	D3DXMatrixScaling(&mScale, width/4.0, height/4.0, depth/4.0);
	D3DXMatrixTranslation(&mTranslate, position.x + targetVector.x*2, position.y + targetVector.y*2, position.z + targetVector.z*2);
	world = world*mScale*mTranslate;
	mWVP = world* (*mVP);
	mfxWVPVar->SetMatrix((float*)&mWVP);
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
        box->draw();
		if(bullet->getActiveState())bullet->draw(mfxWVPVar, mTech, mVP);
    }
}

void Player::update(float dt)
{
	position += velocity*dt;
	//position += targetVector*speed;
	Identity(&world);
	D3DXMatrixScaling(&mScale, width, height, depth);
	D3DXMatrixTranslation(&mTranslate, position.x, position.y, position.z);
	D3DXMatrixRotationY(&mRotate, theta);
	world = world * mScale * mRotate * mTranslate;
	
	//D3DXVECTOR4 tV(0, 0, 0, 0);
	//tV.x = targetVector.x;
	//tV.y = targetVector.y;
	//tV.z = targetVector.z;
	//D3DXVec4Transform(&tV, &tV, &mRotate);
	//targetVector.x = tV.x;
	//targetVector.y = tV.y;
	//targetVector.z = tV.z;

	//D3DXMatrixMultiply(&world, &mScale, &mTranslate);
	//Translate(&world, position.x, position.y, position.z);
	bullet->update(dt);
}

void Player::shoot()
{
	//If the player's got an active bullet on the level, he doesn't get to shoot
	if(bullet->getActiveState() == true) return;

	bullet->setPosition(position);
	bullet->setSpeed(bulletNS::SPEED);
	
	Vector3 nDir(0,0,0);
	
	D3DXVec3Normalize(&nDir, &targetVector);
	nDir *= bulletNS::SPEED;
	bullet->setVelocity(nDir);
	bullet->setActive();
}

void Player::rotateTargeting(int s, float dt)
{
	//theta += 1.0f;
	if(theta > 360) theta -= 360;
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
		D3DXMatrixRotationY(&rotate, ToRadian(-0.25));
		D3DXVec4Transform(&tV, &tV, &rotate);
		theta -= 2.5*dt;
		break;
	case 1:
		D3DXMatrixRotationY(&rotate, ToRadian(0.25));
		D3DXVec4Transform(&tV, &tV, &rotate);
		theta += 2.5*dt;
		break;
	}
	targetVector.x = tV.x;
	targetVector.y = tV.y;
	targetVector.z = tV.z;
	//world *= rotate;
}