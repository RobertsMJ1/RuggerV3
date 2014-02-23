
#ifndef GameObject_H
#define GameObject_H

//#include "d3dUtil.h"
#include "d3dUtil.h"
#include "Box.h"
#include "constants.h"

class GameObject
{
public:

	GameObject();
	~GameObject();

	void init(Box *b, float r, Vector3 pos, Vector3 vel, float sp, float s);
	virtual void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	virtual void update(float dt);

	void setPosition (Vector3 pos) {position = pos;}
	Vector3 getPosition() {return position;}
	void setVelocity (Vector3 vel) {velocity = vel;}
	Vector3 getVelocity() {return velocity;}
	void setSpeed(float s) {speed = s;}
	float getSpeed() {return speed;}
	void setRadius(float r) {radius = r; radiusSquared = (scale*r)*(scale*r);}
	float getRadiusSquare() {return radiusSquared;}
	float getRadius() {return radius;}
	Matrix getWorldMatrix() {return world;}
	void setScale(float s) {scale = s; radiusSquared = (s*radius)*(s*radius);}
	float getScale() {return scale;}
	void setActive() {active = true;}
	void setInActive() {active = false;}
	bool getActiveState() {return active;}
	bool collided(GameObject *gameObject);

	void setRotateX(float rx){rotX = rx;}
	void setRotateY(float ry){rotY = ry;}
	void setRotateZ(float rz){rotZ = rz;}

	//float getWidth(){return scale;}
	virtual float getWidth(){return width;}
	virtual float getHeight(){return height;}
	virtual float getDepth(){return depth;}

private:
	
	float speed;
	
	
protected:
	Box *box;
	Vector3 position;
	Matrix world;
	float scale;
	float rotX, rotY, rotZ;
	bool active;
	float radius;
	float radiusSquared;
	Vector3 velocity;
	int width, height, depth;
	Matrix mTranslate, mRotate, mScale;
};


#endif
