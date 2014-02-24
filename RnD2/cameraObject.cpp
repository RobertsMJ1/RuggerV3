
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

void cameraObject::init(Box *b, float r, Vector3 pos, Vector3 vel, float initRot, float sp, float s)
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
}

void cameraObject::update(float dt)
{
	//position += velocity*dt;
	Identity(&world);
	Matrix translate;
	Matrix scale;
	Matrix rotate;
	Matrix point;

	Scale(&scale, 1, 1, 1.5);
	if (ToRadian(spinAmount*40)< initialRotation+PI/4.0){
		motionHinge = 1;
	}
	if (ToRadian(spinAmount*40)> initialRotation+PI*3.0/4.0){
		motionHinge = -1;
	}
	spinAmount += dt/1.50 * motionHinge;
	RotateZ(&point, ToDegree(-30));
	RotateY(&rotate, ToRadian(spinAmount*40));
	
	Translate(&translate, position.x, position.y, position.z);
	world = scale * rotate * point * translate;
}