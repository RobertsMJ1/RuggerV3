
#include "cameraObject.h"

cameraObject::cameraObject() : GameObject()
{
	radius = 0;
	speed = 0;
	active = true;
	Identity(&world);
	aimVec = Vector3(0,0,0);
}

cameraObject::~cameraObject()
{
	box = NULL;
}

void cameraObject::draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP)
{
	if(!active)return;

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

void cameraObject::init(Box *b, vector<Bullet*> theBullets, float r, Vector3 pos, Vector3 vel, float initRot, float sp, float s)
{
	box = b;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	velocity = vel;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
	spinAmount = initialRotation = initRot;
	motionHinge = 1;
	width = s;
	height = s;
	depth = s;
	timeSinceLastShot = 500;
	cameraObject::bullets = theBullets;
}

void cameraObject::update(float dt, GameObject* player)
{
	//position += velocity*dt;
	Identity(&world);
	Matrix translate;
	Matrix scale;
	Matrix rotate;
	Matrix point;

	aimVec = player->getPosition() - position;
	Scale(&scale, 1, 1, 1.5);
	float fun = 0;
		if(aimVec.z>0)
			fun = asin(sin(aimVec.x/D3DXVec3Length(&aimVec)));
		if(aimVec.z<=0)
			fun = asin(sin(-aimVec.x/D3DXVec3Length(&aimVec)));
	//RotateZ(&point, ToDegree(-30));
	RotateY(&rotate, fun);
	
	Translate(&translate, position.x, position.y, position.z);
	for (int i = 0; i < bullets.size(); i++)
		bullets[i]->update(dt);

	world = scale * rotate * translate;
	timeSinceLastShot+=dt;
}

void cameraObject::shoot(GameObject* player)
{
	if(D3DXVec3LengthSq(&aimVec) > cameraNS::RANGE*cameraNS::RANGE) return;
	if(!active) return;
	if (timeSinceLastShot > 0.5)
		timeSinceLastShot = 0;
	else return;

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
	D3DXVec3Normalize(&aimVec, &aimVec);
	bullets[index]->setVelocity(aimVec);
	bullets[index]->setActive();
	//bullet->setPosition(position);
	//Vector3 aimVec = player->getPosition() - position;
	//if(D3DXVec3Length(&aimVec) > cameraNS::RANGE) return;

	//D3DXVec3Normalize(&aimVec, &aimVec);

	//bullet->setVelocity(aimVec * bulletNS::SPEED);
	//bullet->setActive();
	//
}