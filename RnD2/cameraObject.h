
#ifndef cameraObject_H
#define cameraObject_H

//#include "d3dUtil.h"
#include "d3dUtil.h"
#include "Box.h"
#include "gameObject.h"
#include "constants.h"
#include "gameobject.h"

class cameraObject : public GameObject
{
public:

	cameraObject();
	~cameraObject();

	void init(Box *b, float r, Vector3 pos, Vector3 vel, float initRot, float sp, float s);
	void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	void update(float dt);

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
<<<<<<< HEAD
	//bool collided(cameraObject *cameraObject);
=======
	bool collided(cameraObject *cameraObject);
>>>>>>> Audio Implemented


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

	/*ADDED BY ANDREW */
	float spinAmount;
	float initialRotation;
	int motionHinge;
};


#endif
