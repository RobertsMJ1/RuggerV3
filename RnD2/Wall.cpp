#include "Wall.h"

Wall::Wall()
{
	radius = 0;
	active = true;
	Identity(&world);
	Identity(&mTranslate);
	Identity(&mRotate);
	Identity(&mScale);
	width = 0;
	height = 0;
	depth = 0;
	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
}

Wall::~Wall()
{
	box = NULL;
}

void Wall::draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP)
{
	if (!active)
		return;
	//for(int i=0; i<width; i++){
	//	for(int j=0; j<height; j++){
	//		for(int k=0; k<depth; k++){
	//		//Matrix t;
	//		//Identity(&t);
	//			Translate(&world, position.x+(i*2), position.y+(j*2), position.z+(k*2));
	//			Matrix mWVP = world* (*mVP);
	//			mfxWVPVar->SetMatrix((float*)&mWVP);
	//			D3D10_TECHNIQUE_DESC techDesc;
	//			mTech->GetDesc( &techDesc );
	//			for(UINT p = 0; p < techDesc.Passes; ++p)
	//			{
	//				mTech->GetPassByIndex( p )->Apply(0);
	//				box->draw();
	//			}
	//		}
	//	}
	//}
	//Matrix scale, trans;
	//Identity(&scale);
	////Identity(&trans);
	////D3DXMatrixScaling(&world, width, height, depth);
	////D3DXMatrixTranslation(&trans, position.x, position.y, position.z);
	//world *= scale;
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

void Wall::init(Box *b, float r, Vector3 pos, float s, int w, int h, int d, float rx, float ry, float rz)
{
	box = b;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	scale = s;
	radiusSquared = radius * radius;
	width = w;
	height = h;
	depth = d;
	rotX = rx;
	rotY = ry;
	rotZ = rz;
	//Translate(&world, position.x, position.y, position.z);
}

void Wall::update(float dt)
{
	//position += velocity*dt;
	Identity(&world);
	//Matrix rot, scal, tran;
	//Identity(&rot);
	//Identity(&scal);
	//Identity(&tran);
	D3DXMatrixScaling(&mScale, width, height, depth);
	D3DXMatrixTranslation(&mTranslate, position.x, position.y, position.z);
	D3DXMatrixMultiply(&world, &mScale, &mTranslate);
	//D3DXMatrix
	//D3DXMatrixRotationY(&rot, ToRadian(rotY));
	//Matrix* s = D3DXMatrixScaling(&world, width, height, depth);
	//Matrix* t = Translate(&world, position.x, position.y, position.z);
	//Identity(&world);
	//world = world* (*s) * (*t);
	
	//world *= rot*scal*tran;

}

bool Wall::collided(GameObject *gameObject)
{
	Vector3 diff = position - gameObject->getPosition();
	float length = D3DXVec3LengthSq(&diff);
	float radii = radiusSquared + gameObject->getRadiusSquare();
	if (length <= radii){
		//gameObject->setInActive();
		return true;
	}
	return false;

	//for(int i=0; i<width; i++)
	//{
	//	for(int j=0; j<height; j++)
	//	{
	//		for(int k=0; k<depth; k++)
	//		{

	//		}
	//	}
	//}
	
}