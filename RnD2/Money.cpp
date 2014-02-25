#include "Money.h"


Money::Money(void) : GameObject()
{
	active = true;
	big = false;
}


Money::~Money(void)
{
	box = 0;
}

void Money::init(Box* b, float r, Vector3 pos, Vector3 vel, float sp, float s, bool big)
{
	box = b;
	radius = r;
	radius *= 1.01f;
	position = pos;
	velocity = vel;
	scale = s;
	radiusSquared = radius * radius;
	width = s;
	height = s;
	depth = s;
	active = true;
	Money::big = big;
}

void Money::draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP)
{
	if (!active)
		return;

	
	if(big){		
		D3DXMatrixScaling(&world, 1, 1, 1.5);
		D3DXMatrixTranslation(&world, position.x, position.y ,position.z - 1.25);
		Matrix mWVP = world* (*mVP);
		mfxWVPVar->SetMatrix((float*)&mWVP);
		D3D10_TECHNIQUE_DESC techDesc;
		mTech->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			mTech->GetPassByIndex( p )->Apply(0);
			box->draw();
		}
		D3DXMatrixTranslation(&world, position.x, position.y ,position.z + 1.25);
		mWVP = world* (*mVP);
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mTech->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			mTech->GetPassByIndex( p )->Apply(0);
			box->draw();
		}
		D3DXMatrixTranslation(&world, position.x, position.y + 1, position.z);
		mWVP = world* (*mVP);
		mfxWVPVar->SetMatrix((float*)&mWVP);
		mTech->GetDesc( &techDesc );
		for(UINT p = 0; p < techDesc.Passes; ++p)
		{
			mTech->GetPassByIndex( p )->Apply(0);
			box->draw();
		}
	}
	else 
	{
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
}

void Money::update(float dt)
{
	position += velocity*dt;
	Identity(&world);
	D3DXMatrixScaling(&mScale, width, height, depth);
	D3DXMatrixTranslation(&mTranslate, position.x, position.y, position.z);
	D3DXMatrixMultiply(&world, &mScale, &mTranslate);
	//Translate(&world, position.x, position.y, position.z);
}