#pragma once
#include "gameobject.h"
#include "Bullet.h"

namespace gravNS
{
	const float MASS = 1.0;
}

class Gravball : public GameObject
{
public:
	Gravball(void);
	~Gravball(void);

	//Gravball needs a pointer to just the player's bullet, as the player's bullet is the only thing affected by the gravity ball
	void init(Box* b, Bullet* bullet, float r, Vector3 pos, Vector3 vel, float sp, float s);
	void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	void update(float dt);

private:
	Bullet* bullet;
	float mass;
};

