#pragma once

#include "gameobject.h"

namespace bulletNS {
	const int SPEED = 50;
	const float MASS = 0;
}

class Bullet :	public GameObject
{
public:
	Bullet(void);
	~Bullet(void);

	void init(Box* b, float r, Vector3 pos, Vector3 vel, float sp, float s);
	void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	void update(float dt);
	float getMass(){return mass;}

private:
	float speed;
	float radius;
	float mass;
};

