
#include "GameObject.h"

GameObject::GameObject()
{
	radius = 0;
	speed = 0;
	active = true;
	Identity(&world);
}

GameObject::~GameObject()
{
	box = NULL;
}

void GameObject::draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP)
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
		/*box->draw();*/
}

void GameObject::init(Box *b, float r, Vector3 pos, Vector3 vel, float sp, float s)
{
	box = b;
	radius = r;
	radius *= 1.01; //fudge factor
	position = pos;
	velocity = vel;
	speed = sp;
	scale = s;
	radiusSquared = radius * radius;
}

void GameObject::update(float dt)
{
	position += velocity*dt;
	Identity(&world);
	Translate(&world, position.x, position.y, position.z);

}

//Note that this collision only works for axis-aligned cubes
bool GameObject::collided(GameObject *gameObject)
{
	//Vector3 diff = position - gameObject->getPosition();
	//float length = D3DXVec3LengthSq(&diff);
	//float radii = radiusSquared + gameObject->getRadiusSquare();
	//if (length <= radii){
	//	//gameObject->setInActive();
	//	return true;
	//}
	//return false;

	if(position.x - getWidth() <= gameObject->getPosition().x + gameObject->getWidth() && 
		position.x+getWidth() >= gameObject->getPosition().x - gameObject->getWidth() &&
		position.y - getHeight() <= gameObject->getPosition().y + gameObject->getHeight() &&
		position.y+getHeight() >= gameObject->getPosition().y - gameObject->getHeight() &&
		position.z - getDepth() <= gameObject->getPosition().z + gameObject->getDepth() &&
		position.z+getDepth() >= gameObject->getPosition().z - gameObject->getDepth()) return true;
	else return false;
}