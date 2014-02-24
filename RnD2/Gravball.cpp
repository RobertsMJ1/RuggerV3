#include "Gravball.h"


Gravball::Gravball(void) : GameObject()
{
}


Gravball::~Gravball(void)
{
	box = 0;
	bullet = 0;
}

void Gravball::init(Box* b, Bullet* bullet, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	box = b;
	Gravball::bullet = bullet;
	radius = r;
	radius *= 1.01f;
	position = pos;
	velocity = vel;
	//speed = sp;
	scale = s;
	radiusSquared = radius * radius;
	width = 0;
	height = 0;
	depth = 0;
	mass = gravNS::MASS;
}

void Gravball::draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP)
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
		//if(bullet->getActiveState())bullet->draw(mfxWVPVar, mTech, mVP);
    }
}

void Gravball::update(float dt)
{
	//position += velocity*dt;
	Identity(&world);
	//D3DXMatrixScaling(&mScale, width, height, depth);
	//D3DXMatrixTranslation(&mTranslate, position.x, position.y, position.z);
	//D3DXMatrixMultiply(&world, &mScale, &mTranslate);
	Translate(&world, position.x, position.y, position.z);
	//bullet->update(dt);

    // if either entity is not active then no gravity effect
 //   if (!active || !bullet->getActiveState())
 //       return;

 //   float rr = pow((position.x - bullet->getPosition().x),2) + pow((position.z - bullet->getPosition().z),2);
	//rr *= 1000;
 //   float force = GRAVITY * mass * bullet->getMass()/rr;

 //   // --- Using vector math to create gravity vector ---
 //   // Create vector between entities
 //   Vector3 gravityV(position.x - bullet->getPosition().x, 0, position.z - bullet->getPosition().z);
 //   // Normalize the vector
	//Vector3 nGravityV(0,0,0);
 //   D3DXVec3Normalize(&nGravityV, &gravityV);
 //   // Multipy by force of gravity to create gravity vector
 //   nGravityV *= force * dt;
 //   // Add gravity vector to moving velocity vector to change direction
	//if(D3DXVec3LengthSq(&nGravityV) > pow(500.0, 2))
	//{
	//	D3DXVECTOR3 *gravV = D3DXVec3Normalize(&nGravityV, &nGravityV);
	//	//*gravV *= 500;
	//	*gravV /= 2;
	//	gravityV = *gravV;
	//}
	//
	//bullet->setVelocity(bullet->getVelocity() + gravityV * .9);
}
