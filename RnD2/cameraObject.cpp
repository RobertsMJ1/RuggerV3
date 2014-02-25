
#include "cameraObject.h"

cameraObject::cameraObject() : GameObject()
{
	radius = 0;
	speed = 0;
	active = true;
	Identity(&world);
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
	}
}

void cameraObject::init(Box *b, Bullet* bull, float r, Vector3 pos, Vector3 vel, float initRot, float sp, float s)
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
	bullet = bull;
}

void cameraObject::update(float dt, GameObject* player)
{
	//position += velocity*dt;
	Identity(&world);
	Matrix translate;
	Matrix scale;
	Matrix rotate;
	Matrix point;

	Vector3 aimVec = player->getPosition() - position;
	Scale(&scale, 1, 1, 1.5);
	float fun = 0;
		if(aimVec.z>0)
			fun = asin(sin(aimVec.x/D3DXVec3Length(&aimVec)));
		if(aimVec.z<=0)
			fun = asin(sin(-aimVec.x/D3DXVec3Length(&aimVec)));
	RotateZ(&point, ToDegree(-30));
	RotateY(&rotate, fun);
	
	Translate(&translate, position.x, position.y, position.z);
	world = scale * rotate /* point*/ * translate;
}

void cameraObject::shoot(GameObject* player)
{
	if(canShoot()) return;
	if(!active) return;
	

	bullet->setPosition(position);
	Vector3 aimVec = player->getPosition() - position;
	if(D3DXVec3Length(&aimVec) > cameraNS::RANGE) return;

	D3DXVec3Normalize(&aimVec, &aimVec);

	bullet->setVelocity(aimVec * bulletNS::SPEED);
	bullet->setActive();
	
}