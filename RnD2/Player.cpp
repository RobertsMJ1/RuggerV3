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
	radius *= 1.01;
	position = pos;
	velocity = vel;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
	width = s;
	height = s;
	depth = s;
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
		//bullet->draw(mfxWVPVar, mTech, mVP);
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
	//bullet->update(dt);
}