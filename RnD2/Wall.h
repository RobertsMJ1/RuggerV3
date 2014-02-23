#ifndef WALL_H
#define WALL_H
#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall();
	~Wall();

	//Width and height in integral number of boxes(bricks)
	virtual void init(Box *b, float r, Vector3 pos, float s = 1, int width = 1, int height = 1, int depth = 1, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f);
	virtual void draw(ID3D10EffectMatrixVariable* mfxWVPVar, ID3D10EffectTechnique* mTech, Matrix* mVP);
	virtual void update(float dt);

	//void setPosition (Vector3 pos) {position = pos;}
	//Vector3 getPosition() {return position;}
	//void setRadius(float r) {radius = r; radiusSquared = (scale*r)*(scale*r);}
	//float getRadiusSquare() {return radiusSquared;}
	//float getRadius() {return radius;}
	//Matrix getWorldMatrix() {return world;}
	//void setWorld(Matrix w){world = w;}
	//void setScale(float s) {scale = s; radiusSquared = (s*radius)*(s*radius);}
	//float getScale() {return scale;}
	//void setActive() {active = true;}
	//void setInActive() {active = false;}
	//bool getActiveState() {return active;}
	//bool collided(GameObject *gameObject);

	float getWidth(){
		return width;
	}
	float getHeight(){
		return height;
	}
	float getDepth(){
		return depth;
	}

private:
	float radius;
	float radiusSquared;

	
	
};

#endif