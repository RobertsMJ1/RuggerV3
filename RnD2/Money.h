#pragma once
#include "gameobject.h"
class Money : public GameObject
{
public:
	Money(void);
	~Money(void);

	void init(Box* b, float r, Vector3 pos, Vector3 vel, float sp, float s, bool BIG);
	void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	void update(float dt);
	int getPoints(){return big?1:3;}
	float getDepth(){return big?2*depth:depth;}
private:
	bool big;
};

