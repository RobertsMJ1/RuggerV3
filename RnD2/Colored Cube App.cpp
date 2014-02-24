//=============================================================================
// Color Cube App.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates coloring.
//
// Controls:
//		'A'/'D'/'W'/'S' - Rotate 
//
//=============================================================================


#include "d3dApp.h"
//#include "input.h"
//#include "textDX.h"
#include "Box.h"
#include "GameObject.h"
#include "cameraObject.h"
#include "Line.h"
#include "Quad.h"
#include <d3dx9math.h>
#include "LineObject.h"
#include "Wall.h"
#include "gameError.h"
#include "Player.h"
#include "Bullet.h"
#include "Gravball.h"

namespace gameNS {
	const int NUM_WALLS = 9;
	const int PERIMETER = 4;
	const int NUM_CAMS = 2;
}

class ColoredCubeApp : public D3DApp
{
public:
	ColoredCubeApp(HINSTANCE hInstance);
	~ColoredCubeApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 
	Vector3 moveCube();
	void drawLine(LineObject*);

private:
	void buildFX();
	void buildVertexLayouts();
 
private:
	//Quad quad1;
	Line rLine, bLine, gLine;
	Box mBox, redBox, brick, camBox;
	//GameObject gameObject1/*, gameObject2, gameObject3, spinner, nuBox*/;
	Player player;
	Bullet pBullet;
	//GameObject test;
	LineObject xLine, yLine, zLine;
	//Wall wall;
	//GameObject wall1[50], wall2[50], wall3[50], wall4[50];

	Wall walls[gameNS::NUM_WALLS];
	cameraObject enemyCam[gameNS::NUM_CAMS];
	//Wall perimeter[4];
	Gravball gravball;

	float spinAmount;

	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;
	ID3D10EffectMatrixVariable* mfxWVPVar;
	//my addition
	ID3D10EffectVariable* mfxFLIPVar;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;
	D3DXMATRIX mWVP;
	D3DXMATRIX mVP;

	//my edits
	D3DXMATRIX worldBox1, worldBox2;

	float mTheta;
	float mPhi;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	ColoredCubeApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

ColoredCubeApp::ColoredCubeApp(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mVertexLayout(0),
  mfxWVPVar(0), mTheta(0.0f), mPhi(PI*0.25f)
{
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 
	D3DXMatrixIdentity(&mVP); 
}

ColoredCubeApp::~ColoredCubeApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void ColoredCubeApp::initApp()
{
	D3DApp::initApp();
	
	mBox.init(md3dDevice, 1.0f, WHITE);
	brick.init(md3dDevice, 1.0f, DARKBROWN);
	camBox.init(md3dDevice, 1.0f, BLACK);
	//redBox.init(md3dDevice, 0.00001f, RED);
	rLine.init(md3dDevice, 10.0f, RED);
	bLine.init(md3dDevice, 10.0f, BLACK);
	gLine.init(md3dDevice, 10.0f, GREEN);

	xLine.init(&rLine, Vector3(0,0,0), 5);
	xLine.setPosition(Vector3(0,0,0));
	yLine.init(&bLine, Vector3(0,0,0), 5);
	yLine.setPosition(Vector3(0,0,0));
	yLine.setRotationZ(ToRadian(90));
	zLine.init(&gLine, Vector3(0,0,0), 5);
	zLine.setPosition(Vector3(0,0,0));
	zLine.setRotationY(ToRadian(90));
	
	pBullet.init(&brick, 2.0f, Vector3(0,0,0), Vector3(0,0,0), 0, 1);
	player.init(&mBox, &pBullet, sqrt(2.0f), Vector3(0,0,0), Vector3(0,0,0), 0, 1);
	//test.init(&mBox, sqrt(2.0f), Vector3(10, 0, 10), Vector3(0, 0, 0), 0, 1);
	gravball.init(&mBox, &pBullet, sqrt(2.0f), Vector3(10, 0, 10), Vector3(0,0,0), 0, 1);
	//Initializing the walls' position is completely arbitrary and base on trial-and-error
	walls[0].init(&brick, 2.0f, Vector3(-35,0,-35), 1.0f, 15, 2, 15);
	walls[1].init(&brick, 2.0f, Vector3(-35,0,15), 1.0f, 2, 2, 35);
	walls[2].init(&brick, 2.0f, Vector3(-15,0,25), 1.0f, 2, 2, 25);
	walls[3].init(&brick, 2.0f, Vector3(25,0,0), 1.0f, 20, 2, 2);
	walls[4].init(&brick, 2.0f, Vector3(5,0,-13), 1.0f, 2, 2, 15);
	walls[5].init(&brick, 2.0f, Vector3(0, 0, 50), 1, 50, 10, 1);
	walls[6].init(&brick, 2.0f, Vector3(-50, 0, 0), 1, 1, 2, 50);//Camera-side wall
	walls[7].init(&brick, 2.0f, Vector3(50, 0, 0), 1, 1, 10, 50);
	walls[8].init(&brick, 2.0f, Vector3(0, 0, -50), 1, 50, 10, 1);
	
	enemyCam[0].init(&camBox, 2.0f, Vector3(3,5,0), Vector3(0,0,0), PI/4, 0, 1);
	enemyCam[1].init(&camBox, 2.0f, Vector3(3,5,-4), Vector3(0,0,0), -PI/4, 0, 1);
	buildFX();
	buildVertexLayouts();
}

void ColoredCubeApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void ColoredCubeApp::updateScene(float dt)
{
	
	D3DApp::updateScene(dt);
	Vector3 oldPos = player.getPosition();


	if(input->isKeyDown(VK_UP)) player.shoot(Vector3(1, 0, 0));
	if(input->isKeyDown(VK_DOWN)) player.shoot(Vector3(-1, 0, 0));
	if(input->isKeyDown(VK_LEFT)) player.shoot(Vector3(0, 0, 1));
	if(input->isKeyDown(VK_RIGHT)) player.shoot(Vector3(0, 0, -1));
	gravball.update(dt);
	//test.update(dt);
	player.setSpeed(20);
	player.setVelocity(moveCube() * player.getSpeed());
	player.update(dt);
	//if(player.collided(&test))
	//{
	//	player.setPosition(oldPos);
	//}

	for(int i=0; i<gameNS::NUM_WALLS; i++)
	{
		if(player.collided(&walls[i]))
		{
			player.setPosition(oldPos);
		}
		if(pBullet.collided(&walls[i]))
		{
			pBullet.setInActive();
		}
	}
	
	
	//gameObject2.update(dt);
	//gameObject3.update(dt);
	//spinner.update(dt);
	xLine.update(dt);
	xLine.setPosition(player.getPosition());
	yLine.update(dt);
	yLine.setPosition(player.getPosition());
	zLine.update(dt);
	zLine.setPosition(player.getPosition());
	//wall.update(dt);
	//quad1.update(dt);

	for(int i=0; i<gameNS::NUM_WALLS; i++)walls[i].update(dt);
	for(int i=0; i<gameNS::NUM_CAMS; i++)enemyCam[i].update(dt);
	//spinAmount += dt ;
	//if (ToRadian(spinAmount*40)>2*PI)
	//	spinAmount = 0;

	//Build the view matrix.
	//D3DXVECTOR3 pos(-100.0f,100.0f,50.0f);
	D3DXVECTOR3 pos(-50.0f, 150.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
#pragma region Ray Picking - highly experimental, not working
	//Ray picking - Transform view space -> world space
	float vx = (2.0f*input->getMouseX()/mClientWidth - 1.0f)/mProj._11;
	float vy = (-2.0f*input->getMouseY()/mClientHeight + 1.f)/mProj._22;

	D3DXVECTOR3 rayOrigin(0.f, 0.f, 0.f);
	D3DXVECTOR3 rayDir(vx, vy, 1.f);
	D3DXMATRIX inverseV;
	D3DXMatrixInverse(&inverseV, 0, &mView);

	D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &inverseV);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &inverseV);

	Vector3 pNormal(0, 1, 0);
	Vector3 pPoint(1, 0, 0);
	float t = D3DXVec3Dot(&(-1*pNormal),&pPoint)/D3DXVec3Dot(&pNormal, &rayDir);
	Vector3 intersectionPoint = rayOrigin + t*rayDir;

	//xLine.setPosition(intersectionPoint);
#pragma endregion
}

void ColoredCubeApp::drawScene()
{
	D3DApp::drawScene();

	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);

	// set some variables for the shader
	int foo[1];
	foo[0] = 0;
	// set the point to the shader technique
	D3D10_TECHNIQUE_DESC techDesc;
	mTech->GetDesc(&techDesc);

	//Set mVP to be view*projection, so we can pass that into GO::draw(..)
	mVP = mView*mProj;

	//setting the color flip variable in the shader
	mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));

	//draw the lines
	drawLine(&xLine);
	drawLine(&yLine);
	drawLine(&zLine);
	
	/*****************************************
	Walls!
	*******************************************/
	for(int i=0; i<gameNS::NUM_WALLS; i++)walls[i].draw(mfxWVPVar, mTech, &mVP);

	////draw the boxes
	//test.draw(mfxWVPVar, mTech, &mVP);
	player.draw(mfxWVPVar, mTech, &mVP);
	//pBullet.draw(mfxWVPVar, mTech, &mVP);
	//Player & bullet classes implemented
	gravball.draw(mfxWVPVar, mTech, &mVP);
	//gameObject1.draw(mfxWVPVar, mTech, &mVP);	

	/*****************************************
	Enemy Cameras
	*******************************************/
	for(int i=0; i<gameNS::NUM_CAMS; i++)enemyCam[i].draw(mfxWVPVar, mTech, &mVP);

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = {5, 5, 0, 0};
	mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
	mSwapChain->Present(0, 0);
}

void ColoredCubeApp::drawLine(LineObject* line) {
	mWVP = line->getWorldMatrix()*mView*mProj;
	mfxWVPVar->SetMatrix((float*)&mWVP);
	line->setMTech(mTech);
	line->draw();
}

void ColoredCubeApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
 
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"color.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName("ColorTech");
	
	mfxWVPVar = mFX->GetVariableByName("gWVP")->AsMatrix();
	mfxFLIPVar = mFX->GetVariableByName("flip");

}

void ColoredCubeApp::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 2, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
}

Vector3 ColoredCubeApp::moveCube()
{
	Vector3 dir = Vector3(0,0,0);

	if(input->isKeyDown(KEY_W))
	{
		dir.x = 1;
	}
	if(input->isKeyDown(KEY_S)) dir.x = -1;
	if(input->isKeyDown(KEY_A)) dir.z = 1;
	if(input->isKeyDown(KEY_D)) dir.z = -1;

	if(dir != Vector3(0,0,0))
			D3DXVec3Normalize(&dir, &dir);

	return dir;
}