#pragma once
#include "LineObject.h"

class Origin
{
public:
	Origin();
	~Origin();

	void init(LineObject* l);
	void draw();
	void update(float dt);

	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}

	Matrix getWorldMatrix() {return world;}

	void setActive() {active = true;}
	void setInActive() {active = false;}
	bool getActiveState() {return active;}

	void setMTech(ID3D10EffectTechnique* m){ mTech = m;}

	void setRotationX(float r) {rotX = r;}
	void setRotationY(float r) {rotY = r;}
	void setRotationZ(float r) {rotZ = r;}

private:
	LineObject* baseLine;
	Vector3 position;
	bool active;
	Matrix world;
	ID3D10EffectTechnique* mTech;
	float rotX, rotY, rotZ;
	
};

