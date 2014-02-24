#include "Bullet.h"


Bullet::Bullet(void) : GameObject()
{
	active = false;
}


Bullet::~Bullet(void)
{
}

void Bullet::init(Box* b, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	box = b;
	radius = r;
	radius *= 1.01;
	position = pos;
	velocity = vel;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
	width = height = depth = scale;
}

void Bullet::draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP)
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
    }
}

void Bullet::update(float dt)
{
	position += velocity*dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);
	//D3DXMatrixScaling(&mScale, width, height, depth);
	//D3DXMatrixTranslation(&mTranslate, position.x, position.y, position.z);
	//D3DXMatrixMultiply(&world, &mScale, &mTranslate);
}