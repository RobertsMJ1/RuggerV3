#include "Origin.h"


Origin::Origin()
{
	active = true;
	Identity(&world);
	rotX = 0;
	rotY = 0;
	rotZ = 0;
}


Origin::~Origin()
{
	baseLine = NULL;
}

void Origin::draw()
{
	if (!active)
		return;
    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
	

	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex( p )->Apply(0);
		baseLine->draw();
	}

	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex( p )->Apply(0);
		baseLine->draw();
	}

	for(UINT p = 0; p < techDesc.Passes; ++p)
	{
		mTech->GetPassByIndex( p )->Apply(0);
		baseLine->draw();
	}

}

void Origin::init(LineObject* l)
{
	baseLine = l;
	//baseLine->init(
	position = Vector3(0.0f, 0.0f, 0.0f);
	rotX = 0;
	rotY = 0;
	rotZ = 0;
}

void Origin::update(float dt)
{

	Matrix rotXM, rotYM, rotZM, transM;
	RotateX(&rotXM, rotX);
	RotateY(&rotYM, rotY);
	RotateZ(&rotZM, rotZ); 
	Translate(&transM, position.x, position.y, position.z);
	world = rotXM * rotYM * rotZM * transM;
}
