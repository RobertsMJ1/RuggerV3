
#ifndef cameraObject_H
#define cameraObject_H

//#include "d3dUtil.h"
#include "d3dUtil.h"
#include "Box.h"
#include "GameObject.h"
#include "constants.h"
#include "Bullet.h"
#include <vector>
using std::vector;

namespace cameraNS {
	const float RANGE = 30;
}

class cameraObject : public GameObject
{
public:

	cameraObject();
	~cameraObject();

	void init(Box *b, vector<Bullet*> theBullets, float r, Vector3 pos, Vector3 vel, float initRot, float sp, float s);
	void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	void update(float dt, GameObject* player);
	void shoot(GameObject* player);
	//bool canShoot(){return bullets->getActiveState();}
	bool isInRange(Vector3 aimVec){aimVec-=position; if(D3DXVec3Length(&aimVec) > cameraNS::RANGE) return false; else return true;}
	//void setPosition (Vector3 pos) {position = pos;}
	//Vector3 getPosition() {return position;}
	//void setVelocity (Vector3 vel) {velocity = vel;}
	//Vector3 getVelocity() {return velocity;}
	//void setSpeed(float s) {speed = s;}
	//float getSpeed() {return speed;}
	//void setRadius(float r) {radius = r; radiusSquared = (scale*r)*(scale*r);}
	//float getRadiusSquare() {return radiusSquared;}
	//float getRadius() {return radius;}
	//Matrix getWorldMatrix() {return world;}
	//void setScale(float s) {scale = s; radiusSquared = (s*radius)*(s*radius);}
	//float getScale() {return scale;}
	//void setActive() {active = true;}
	//void setInActive() {active = false;}
	//bool getActiveState() {return active;}
	//void setMTech(ID3D10EffectTechnique* m){ mTech = m;}
	//bool collided(cameraObject *cameraObject);


private:
	//Box *box; 
	//Vector3 position;
	//Vector3 velocity;
	//Matrix sca;
	//Matrix rot;
	float speed;
	//float radius;
	//float radiusSquared;
	//bool active;
	//Matrix world;
	//float scale;
	//int piece;
	//ID3D10EffectTechnique* mTech;
	double timeSinceLastShot;
	vector<Bullet*> bullets;

	/*ADDED BY ANDREW */
	float spinAmount;
	float initialRotation;
	int motionHinge;
	Vector3 aimVec;
};


#endif
