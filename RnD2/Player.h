#pragma once
#include "gameobject.h"
#include "Bullet.h"

class Player :
	public GameObject
{
public:
	Player(void);
	~Player(void);

	//Player takes a pointer to a bullet which will be handled completely by the player class: update, drawing, and all
	void init(Box* b, Bullet* bullet, float r, Vector3 pos, Vector3 vel, float sp, float s);
	void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	void update(float dt);

	void shoot(Vector3 direction);

private:
	float speed;
	float radius;

	Bullet* bullet;
};

