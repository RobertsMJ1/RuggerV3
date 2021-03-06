/*



			THIS IS RUGGER AND DUNSTAN 2.0!
			..Sorry, no grav balls in this one.

			wasd to move, arrows to shoot
			uncomment the following define to engage no-clip mode

			
*/

//#define DEBUGGING //DR. BIRMINGHAM MODE


























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
#include "debugText.h"
#include "Audio.h"
#include "Money.h"
#include <ctime>
using std::time;

namespace colorNS
{
    // Some common colors
    // ARGB numbers range from 0 through 255
    // A = Alpha channel (transparency where 255 is opaque)
    // R = Red, G = Green, B = Blue
    const COLOR_ARGB ORANGE  = D3DCOLOR_ARGB(255,255,165,  0);
    const COLOR_ARGB BROWN   = D3DCOLOR_ARGB(255,139, 69, 19);
    const COLOR_ARGB LTGRAY  = D3DCOLOR_ARGB(255,192,192,192);
    const COLOR_ARGB GRAY    = D3DCOLOR_ARGB(255,128,128,128);
    const COLOR_ARGB OLIVE   = D3DCOLOR_ARGB(255,128,128,  0);
    const COLOR_ARGB PURPLE  = D3DCOLOR_ARGB(255,128,  0,128);
    const COLOR_ARGB MAROON  = D3DCOLOR_ARGB(255,128,  0,  0);
    const COLOR_ARGB TEAL    = D3DCOLOR_ARGB(255,  0,128,128);
    const COLOR_ARGB GREEN   = D3DCOLOR_ARGB(255,  0,128,  0);
    const COLOR_ARGB NAVY    = D3DCOLOR_ARGB(255,  0,  0,128);
    const COLOR_ARGB WHITE   = D3DCOLOR_ARGB(255,255,255,255);
    const COLOR_ARGB YELLOW  = D3DCOLOR_ARGB(255,255,255,  0);
    const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255,255,  0,255);
    const COLOR_ARGB RED     = D3DCOLOR_ARGB(255,255,  0,  0);
    const COLOR_ARGB CYAN    = D3DCOLOR_ARGB(255,  0,255,255);
    const COLOR_ARGB LIME    = D3DCOLOR_ARGB(255,  0,255,  0);
    const COLOR_ARGB BLUE    = D3DCOLOR_ARGB(255,  0,  0,255);
    const COLOR_ARGB BLACK   = D3DCOLOR_ARGB(255,  0,  0,  0);
    const COLOR_ARGB FILTER  = D3DCOLOR_ARGB(  0,  0,  0,  0);  // use to specify drawing with colorFilter
    const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB( 64,255,255,255);  // AND with color to get 25% alpha
    const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128,255,255,255);  // AND with color to get 50% alpha
    const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game
    const COLOR_ARGB TRANSCOLOR = MAGENTA;                      // transparent color

    enum DISPLAY_MODE{TOGGLE, FULLSCREEN, WINDOW};
}

namespace gameNS {
	const int NUM_WALLS = 41;
	const int PERIMETER = 4;
	const int NUM_CAMS = 78;
	const int NUM_MONEY = 2500;
	const int NUM_BULLETS = 5;
	const int NUM_RAGE_PICKUPS = 4;
}

namespace fontNS
{
    const char DEBUGFONT[] = "Courier New";
	const char NORMALFONT[] = "Arial Bold";
    const int FONT_HEIGHT = 14;         //height in pixels
	const int FONT_BIG_SIZE = 255;
	const COLOR_ARGB FONT_COLOR = colorNS::WHITE;
	
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
	Line rLine, bLine, gLine;
	Box mBox, redBox, brick, camBox, bulletBox, eBulletBox, yellowGreenBox, goldBox, blueBox, tealBox, maroonBox;
	Player player;
	vector<Bullet*> pBullets, enBullets[gameNS::NUM_CAMS];
	LineObject xLine, yLine, zLine;
	Wall walls[gameNS::NUM_WALLS];
	cameraObject enemyCam[gameNS::NUM_CAMS];
	//Bullet enBullet[gameNS::NUM_CAMS];
	//Gravball gravball;
	Wall floor;
	Money money[gameNS::NUM_MONEY];
	vector<GameObject> ragePickups;
	GameObject superLowFloorOffInTheDistanceUnderTheScene;

	float spinAmount;
	int shotTimer;
	int enemyTimer[gameNS::NUM_CAMS];
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
	int incrementedYMargin;

	int score;
	bool firstpass;
	bool startScreen, endScreen;
	DebugText sText, eText;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
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
	srand(time(0));
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProj);
	D3DXMatrixIdentity(&mWVP); 
	D3DXMatrixIdentity(&mVP); 
	score = 0;
	firstpass = true;
	startScreen = true;
	endScreen = false;
}

ColoredCubeApp::~ColoredCubeApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);

	//CHANGE if audio needs to be released in d3dApp instead
	safeDelete(audio);
  // 
}

void ColoredCubeApp::initApp()
{
	D3DApp::initApp();
#pragma region Base object initialization
	mBox.init(md3dDevice, 1.0f, WHITE);
	tealBox.init(md3dDevice, 1.0f, colorNS::TEAL);
	brick.init(md3dDevice, 1.0f, DARKBROWN);
	camBox.init(md3dDevice, 1.0f, BLACK);
	bulletBox.init(md3dDevice, 0.5f, BEACH_SAND);
	eBulletBox.init(md3dDevice, 0.5f, RED);
	maroonBox.init(md3dDevice, 10000, colorNS::MAROON);
	//redBox.init(md3dDevice, 0.00001f, RED);
	yellowGreenBox.init(md3dDevice, 1.f, LIGHT_YELLOW_GREEN);
	goldBox.init(md3dDevice, 1.0f, YELLOW);
	blueBox.init(md3dDevice, 2.0f, BLUE);
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

	for (int i = 0; i < gameNS::NUM_BULLETS; i++) {
		pBullets.push_back(new Bullet());
		pBullets[i]->init(&bulletBox, 2.0f, Vector3(0,0,0), Vector3(0,0,0), 0, 1);
	}
	for(int j=0; j<gameNS::NUM_CAMS; j++){
		for (int i = 0; i < gameNS::NUM_BULLETS; i++) {
			enBullets[j].push_back(new Bullet());
			enBullets[j][i]->init(&eBulletBox, 2.0f, Vector3(0,0,0), Vector3(0,0,0), 0, 1);
		}
		enemyTimer[j]= 0;
	}

	for (int i = 0; i < gameNS::NUM_RAGE_PICKUPS; i++) {
		ragePickups.push_back(GameObject());
	}

	
	ragePickups[0].init(&tealBox, 2.0f, Vector3(-35,0,95), Vector3(0,0,0),0,1);
	ragePickups[1].init(&tealBox, 2.0f, Vector3(6,0,-80), Vector3(0,0,0),0,1);
	ragePickups[2].init(&tealBox, 2.0f, Vector3(-84,0,-56), Vector3(0,0,0),0,1);
	ragePickups[3].init(&tealBox, 2.0f, Vector3(10,0,80), Vector3(0,0,0),0,1);
	

	//floor.init(&yellowGreenBox, sqrt(2.0), Vector3(-5,-0.02,-5), Vector3(0,0,0), 0, 1);
	floor.init(&yellowGreenBox, 2.0f, Vector3(0,-1.5f,0), 1.0f, 100, 0.01, 100);
	player.init(&mBox, pBullets, sqrt(2.0f), Vector3(-90,0,85), Vector3(0,0,0), 0, 1);
	
	superLowFloorOffInTheDistanceUnderTheScene.init(&maroonBox, 2.0f, Vector3(0,-10.0f,0), Vector3(0,0,0), 0, 100000);

//				   geom,  rad,  position,			sc,	w,  h,  d
	walls[0].init(&brick, 2.0f, Vector3(0, 0, 100),	1, 100, 10, 1);
	walls[1].init(&brick, 2.0f, Vector3(0, 0, -100),1, 100, 10, 1);
	walls[2].init(&brick, 2.0f, Vector3(100, 0, 0), 1, 1,	10, 100);
	walls[3].init(&brick, 2.0f, Vector3(-100, 0, 0),1, 1,	2, 100);
	walls[4].init(&brick, 2.0f, Vector3(-80,0,75),	1, 1,	2,	25);
	walls[5].init(&brick, 2.0f, Vector3(-80,0,25),	1, 20,	2,	1);
	walls[6].init(&brick, 2.0f, Vector3(-53.5,0,0),	1, 46.5,2,	1);
	walls[7].init(&brick, 2.0f, Vector3(-30,0, 25),	1, 1,	2,  25);
	walls[8].init(&brick, 2.0f, Vector3(-10,0, 50),	1, 10,	2,  1);
	walls[9].init(&brick, 2.0f, Vector3(0,0, 74),	1, 1,	2,  26);
	walls[10].init(&brick, 2.0f, Vector3(-5,0, 12),	1, 25,	2,  1);
	walls[11].init(&brick, 2.0f, Vector3(28.5,0, 0),1,21.5,2,  1);
	walls[12].init(&brick, 2.0f, Vector3(0.5,0, -37), 1, 1,	2,  37);
	walls[13].init(&brick, 2.0f, Vector3(20,0, 40), 1, 1,	2,  40);
	walls[14].init(&brick, 2.0f, Vector3(50,0, 80), 1, 30,	2,  1);
	walls[15].init(&brick, 2.0f, Vector3(75,0, 60), 1, 25,	2,  1);
	walls[16].init(&brick, 2.0f, Vector3(50,0, 40), 1, 30,	2,  1);
	walls[17].init(&brick, 2.0f, Vector3(75,0, 20), 1, 25,	2,  1);
	walls[18].init(&brick, 2.0f, Vector3(50,0, 10), 1, 30,	2,  1);
	walls[19].init(&brick, 2.0f, Vector3(80,0, 0), 1, 20,	2,  1);
	walls[20].init(&brick, 2.0f, Vector3(48.5,0, -30), 1, 1,	2,  30);
	walls[21].init(&brick, 2.0f, Vector3(60,0, -22.5), 1, 1,	2,  23);
	walls[22].init(&brick, 2.0f, Vector3(60, 0, -60), 1, 11.5,2,  1);
	walls[23].init(&brick, 2.0f, Vector3(-15, 0, -75), 1, 1, 2, 25);
	walls[24].init(&brick, 2.0f, Vector3(-40, 0, -50), 1, 40, 2, 1);
	walls[25].init(&brick, 2.0f, Vector3(-30, 0, -35), 1, 20, 2, 1);
	walls[26].init(&brick, 2.0f, Vector3(-30, 0, -15), 1, 20, 2, 1);
	walls[27].init(&brick, 2.0f, Vector3(-30, 0, -8), 1, 1, 2, 8);
	walls[28].init(&brick, 2.0f, Vector3(-30, 0, -42), 1, 1, 2, 8);
	walls[29].init(&brick, 2.0f, Vector3(-60, 0, -31), 1, 1, 2, 18);
	walls[30].init(&brick, 2.0f, Vector3(-70, 0, -31), 1, 1, 2, 18);
	walls[31].init(&brick, 2.0f, Vector3(-79, 0, -30), 1, 1, 2, 19);
	walls[32].init(&brick, 2.0f, Vector3(-79, 0, -85), 1, 1, 2, 15);
	walls[33].init(&brick, 2.0f, Vector3(-95, 0, -70), 1, 5, 2, 1);
	walls[34].init(&brick, 2.0f, Vector3(-95, 0, -30), 1, 5, 2, 1);
	walls[35].init(&brick, 2.0f, Vector3(80, 0, -25), 1, 1, 2, 25);
	walls[36].init(&brick, 2.0f, Vector3(48.5, 0, -70), 1, 1, 2, 15);
	walls[37].init(&brick, 2.0f, Vector3(29.5, 0, -85), 1, 20, 2, 1);
	walls[38].init(&brick, 2.0f, Vector3(-50, 0, 49), 1, 20, 2, 1);
	walls[39].init(&brick, 2.0f, Vector3(-50, 0, 70), 1, 20, 2, 1);
	walls[40].init(&brick, 2.0f, Vector3(-30, 0, 70), 1, 1, 2, 20);
#pragma endregion

	//for(int i=0; i<gameNS::NUM_CAMS; i++)
	//{
	//	enBullet[i].init(&eBulletBox, 2.0f, Vector3(0,0,0), Vector3(0,0,0), bulletNS::SPEED, 1);
	//	
	//}
#pragma region Camera Placement
	enemyCam[0].init(&camBox, enBullets[0], 2.0f, Vector3(-90,0,15), Vector3(0,0,0), 0, 0, 1);
	enemyCam[1].init(&camBox, enBullets[1], 2.0f, Vector3(-35,0,5), Vector3(0,0,0), 0, 0, 1);
	enemyCam[2].init(&camBox, enBullets[2], 2.0f, Vector3(-35,0,45), Vector3(0,0,0), 0, 0, 1);
	enemyCam[3].init(&camBox, enBullets[3], 2.0f, Vector3(-35,0,60), Vector3(0,0,0), 0, 0, 1);
	enemyCam[4].init(&camBox, enBullets[4], 2.0f, Vector3(-75,0,60), Vector3(0,0,0), 0, 0, 1);
	enemyCam[5].init(&camBox, enBullets[5], 2.0f, Vector3(-35,0,75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[6].init(&camBox, enBullets[6], 2.0f, Vector3(-75,0,95), Vector3(0,0,0), 0, 0, 1);
	enemyCam[7].init(&camBox, enBullets[7], 2.0f, Vector3(-5,0,95), Vector3(0,0,0), 0, 0, 1);
	enemyCam[8].init(&camBox, enBullets[8], 2.0f, Vector3(-5,0,55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[9].init(&camBox, enBullets[9], 2.0f, Vector3(-25,0,75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[10].init(&camBox, enBullets[10], 2.0f, Vector3(15,0,16), Vector3(0,0,0), 0, 0, 1);
	enemyCam[11].init(&camBox, enBullets[11], 2.0f, Vector3(-25,0,16), Vector3(0,0,0), 0, 0, 1);
	enemyCam[12].init(&camBox, enBullets[12], 2.0f, Vector3(0,0,45), Vector3(0,0,0), 0, 0, 1);
	enemyCam[13].init(&camBox, enBullets[13], 2.0f, Vector3(10,0,75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[14].init(&camBox, enBullets[14], 2.0f, Vector3(30,0,90), Vector3(0,0,0), 0, 0, 1);
	enemyCam[15].init(&camBox, enBullets[15], 2.0f, Vector3(60,0,90), Vector3(0,0,0), 0, 0, 1);
	enemyCam[16].init(&camBox, enBullets[16], 2.0f, Vector3(90,0,90), Vector3(0,0,0), 0, 0, 1);
	enemyCam[17].init(&camBox, enBullets[17], 2.0f, Vector3(30,0,70), Vector3(0,0,0), 0, 0, 1);
	enemyCam[18].init(&camBox, enBullets[18], 2.0f, Vector3(60,0,70), Vector3(0,0,0), 0, 0, 1);
	enemyCam[19].init(&camBox, enBullets[19], 2.0f, Vector3(90,0,70), Vector3(0,0,0), 0, 0, 1);
	enemyCam[20].init(&camBox, enBullets[20], 2.0f, Vector3(30,0,50), Vector3(0,0,0), 0, 0, 1);
	enemyCam[21].init(&camBox, enBullets[21], 2.0f, Vector3(60,0,50), Vector3(0,0,0), 0, 0, 1);
	enemyCam[22].init(&camBox, enBullets[22], 2.0f, Vector3(90,0,50), Vector3(0,0,0), 0, 0, 1);
	enemyCam[23].init(&camBox, enBullets[23], 2.0f, Vector3(45,0,60), Vector3(0,0,0), 0, 0, 1);
	enemyCam[24].init(&camBox, enBullets[24], 2.0f, Vector3(30,0,30), Vector3(0,0,0), 0, 0, 1);
	enemyCam[25].init(&camBox, enBullets[25], 2.0f, Vector3(60,0,30), Vector3(0,0,0), 0, 0, 1);
	enemyCam[26].init(&camBox, enBullets[26], 2.0f, Vector3(90,0,30), Vector3(0,0,0), 0, 0, 1);
	enemyCam[27].init(&camBox, enBullets[27], 2.0f, Vector3(90,0,10), Vector3(0,0,0), 0, 0, 1);
	enemyCam[28].init(&camBox, enBullets[28], 2.0f, Vector3(30,0,5), Vector3(0,0,0), 0, 0, 1);
	enemyCam[29].init(&camBox, enBullets[29], 2.0f, Vector3(70,0,-20), Vector3(0,0,0), 0, 0, 1);
	enemyCam[30].init(&camBox, enBullets[30], 2.0f, Vector3(90,0,-20), Vector3(0,0,0), 0, 0, 1);
	enemyCam[31].init(&camBox, enBullets[31], 2.0f, Vector3(90,0,-90), Vector3(0,0,0), 0, 0, 1);
	enemyCam[32].init(&camBox, enBullets[32], 2.0f, Vector3(55,0,-65), Vector3(0,0,0), 0, 0, 1);
	enemyCam[33].init(&camBox, enBullets[33], 2.0f, Vector3(30,0,-92), Vector3(0,0,0), 0, 0, 1);
	enemyCam[34].init(&camBox, enBullets[34], 2.0f, Vector3(-8,0,-60), Vector3(0,0,0), 0, 0, 1);
	enemyCam[35].init(&camBox, enBullets[35], 2.0f, Vector3(10,0,-5), Vector3(0,0,0), 0, 0, 1);
	enemyCam[36].init(&camBox, enBullets[36], 2.0f, Vector3(10,0,-25), Vector3(0,0,0), 0, 0, 1);
	enemyCam[37].init(&camBox, enBullets[37], 2.0f, Vector3(10,0,-45), Vector3(0,0,0), 0, 0, 1);
	enemyCam[38].init(&camBox, enBullets[38], 2.0f, Vector3(10,0,-65), Vector3(0,0,0), 0, 0, 1);
	enemyCam[39].init(&camBox, enBullets[39], 2.0f, Vector3(20,0,-15), Vector3(0,0,0), 0, 0, 1);
	enemyCam[40].init(&camBox, enBullets[40], 2.0f, Vector3(20,0,-35), Vector3(0,0,0), 0, 0, 1);
	enemyCam[41].init(&camBox, enBullets[41], 2.0f, Vector3(20,0,-55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[42].init(&camBox, enBullets[42], 2.0f, Vector3(20,0,-75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[43].init(&camBox, enBullets[43], 2.0f, Vector3(30,0,-5), Vector3(0,0,0), 0, 0, 1);
	enemyCam[44].init(&camBox, enBullets[44], 2.0f, Vector3(30,0,-25), Vector3(0,0,0), 0, 0, 1);
	enemyCam[45].init(&camBox, enBullets[45], 2.0f, Vector3(30,0,-45), Vector3(0,0,0), 0, 0, 1);
	enemyCam[46].init(&camBox, enBullets[46], 2.0f, Vector3(30,0,-65), Vector3(0,0,0), 0, 0, 1);
	enemyCam[47].init(&camBox, enBullets[47], 2.0f, Vector3(40,0,-15), Vector3(0,0,0), 0, 0, 1);
	enemyCam[48].init(&camBox, enBullets[48], 2.0f, Vector3(40,0,-35), Vector3(0,0,0), 0, 0, 1);
	enemyCam[49].init(&camBox, enBullets[49], 2.0f, Vector3(40,0,-55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[50].init(&camBox, enBullets[50], 2.0f, Vector3(40,0,-75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[51].init(&camBox, enBullets[51], 2.0f, Vector3(-25,0,6), Vector3(0,0,0), 0, 0, 1);
	enemyCam[52].init(&camBox, enBullets[52], 2.0f, Vector3(-25,0,-6), Vector3(0,0,0), 0, 0, 1);
	enemyCam[53].init(&camBox, enBullets[53], 2.0f, Vector3(-25,0,-43), Vector3(0,0,0), 0, 0, 1);
	enemyCam[54].init(&camBox, enBullets[54], 2.0f, Vector3(-35,0,-6), Vector3(0,0,0), 0, 0, 1);
	enemyCam[55].init(&camBox, enBullets[55], 2.0f, Vector3(-35,0,-43), Vector3(0,0,0), 0, 0, 1);
	enemyCam[56].init(&camBox, enBullets[56], 2.0f, Vector3(-50,0,-25), Vector3(0,0,0), 0, 0, 1);
	enemyCam[57].init(&camBox, enBullets[57], 2.0f, Vector3(-10,0,-25), Vector3(0,0,0), 0, 0, 1);
	enemyCam[58].init(&camBox, enBullets[58], 2.0f, Vector3(-65,0,-45), Vector3(0,0,0), 0, 0, 1);
	enemyCam[59].init(&camBox, enBullets[59], 2.0f, Vector3(-75,0,-45), Vector3(0,0,0), 0, 0, 1);
	enemyCam[60].init(&camBox, enBullets[60], 2.0f, Vector3(-95,0,-15), Vector3(0,0,0), 0, 0, 1);
	enemyCam[61].init(&camBox, enBullets[61], 2.0f, Vector3(-95,0,-50), Vector3(0,0,0), 0, 0, 1);
	enemyCam[62].init(&camBox, enBullets[62], 2.0f, Vector3(-95,0,-95), Vector3(0,0,0), 0, 0, 1);
	enemyCam[63].init(&camBox, enBullets[63], 2.0f, Vector3(-33,0,-95), Vector3(0,0,0), 0, 0, 1);
	enemyCam[64].init(&camBox, enBullets[64], 2.0f, Vector3(-46,0,-95), Vector3(0,0,0), 0, 0, 1);
	enemyCam[65].init(&camBox, enBullets[65], 2.0f, Vector3(-59,0,-95), Vector3(0,0,0), 0, 0, 1);
	enemyCam[66].init(&camBox, enBullets[66], 2.0f, Vector3(-72,0,-95), Vector3(0,0,0), 0, 0, 1);
	enemyCam[67].init(&camBox, enBullets[67], 2.0f, Vector3(-20,0,-75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[68].init(&camBox, enBullets[68], 2.0f, Vector3(-33,0,-75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[69].init(&camBox, enBullets[69], 2.0f, Vector3(-46,0,-75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[70].init(&camBox, enBullets[70], 2.0f, Vector3(-59,0,-75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[71].init(&camBox, enBullets[71], 2.0f, Vector3(-72,0,-75), Vector3(0,0,0), 0, 0, 1);
	enemyCam[72].init(&camBox, enBullets[72], 2.0f, Vector3(-20,0,-55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[73].init(&camBox, enBullets[73], 2.0f, Vector3(-33,0,-55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[74].init(&camBox, enBullets[74], 2.0f, Vector3(-46,0,-55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[75].init(&camBox, enBullets[75], 2.0f, Vector3(-59,0,-55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[76].init(&camBox, enBullets[76], 2.0f, Vector3(-72,0,-55), Vector3(0,0,0), 0, 0, 1);
	enemyCam[77].init(&blueBox, enBullets[77], 2.0f, Vector3(-20,0,-95), Vector3(0,0,0), 0, 0, 1); //dunstan will always be the last camera in the list
#pragma endregion

	for(int i=0; i<gameNS::NUM_MONEY; i++)
	{
		money[i].init(&goldBox, 2.0f, Vector3(rand()%190 - 90, 0, rand()%180 - 90), Vector3(0,0,0), 0, 1, rand()%2);
	}

	//Initializing text strings yay
	sText.addLine("WELCOME RUGGER !", 10, 10);
	sText.addLine("WASD TO MOVE", 10, 30);
	sText.addLine("ARROW KEYS TO SHOOT", 10, 50);
	sText.addLine("HOLD SHIFT TO SPRINT!", 10, 70);
	sText.addLine("GO KILL DUNSTAN FOR ME!", 10, 90);
	sText.addLine("PRESS ANY KEY TO BEGIN !", 250, 300);
	eText.addLine("CONGRATS RUGGER I WON!", 250, 300);

	shotTimer = 0;
	buildFX();
	buildVertexLayouts();
	audio->playCue(MUSIC);
}

void ColoredCubeApp::onResize()
{
	D3DApp::onResize();

	float aspect = (float)mClientWidth/mClientHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void ColoredCubeApp::updateScene(float dt)
{
	if(!endScreen && !startScreen)
	{
	
		if(!enemyCam[gameNS::NUM_CAMS-1].getActiveState()) endScreen = true;
	
		D3DApp::updateScene(dt);
		Vector3 oldPos = player.getPosition();

		if(input->isKeyDown(VK_UP)) player.shoot(UP);
		if(input->isKeyDown(VK_DOWN)) player.shoot(DOWN);
		if(input->isKeyDown(VK_LEFT)) player.shoot(LEFT);
		if(input->isKeyDown(VK_RIGHT)) player.shoot(RIGHT);
		if(input->isKeyDown(VK_SHIFT)) {
			player.setSpeed(40);
		}
		else player.setSpeed(20);
		player.setVelocity(moveCube() * player.getSpeed());
		player.update(dt);
	
		for (int i = 0; i < ragePickups.size(); i++) {
			if (player.collided(&ragePickups[i])) {
				ragePickups[i].setInActive();
				player.charge();
			}
			ragePickups[i].update(dt);
		}

		for(int i=0; i<gameNS::NUM_WALLS; i++)
		{
			if(player.collided(&walls[i]))
			{
				//DEBUGGING AND LEVEL LAYOUT, COMMENT THIS OUT
#ifdef DEBUGGING
				
#else
				player.setPosition(oldPos);
#endif
			}
			for (int j = 0; j < pBullets.size(); j++) {
				if (pBullets[j]->collided(&walls[i])) {
					pBullets[j]->setInActive();
					pBullets[j]->setVelocity(D3DXVECTOR3(0,0,0));
					pBullets[j]->setPosition(D3DXVECTOR3(0,0,0));
					shotTimer = 0;
				}		
			}
			for(int j=0; j<gameNS::NUM_CAMS; j++)
			{
				for(int k=0; k<enBullets[j].size(); k++)
				{
					if(enBullets[j][k]->collided(&walls[i])) 
					{
						enBullets[j][k]->setInActive();
						enBullets[j][k]->setVelocity(Vector3(0,0,0));
					}
					if(player.collided(enBullets[j][k]))
					{
						enBullets[j][k]->setInActive();
						enBullets[j][k]->setVelocity(Vector3(0,0,0));
						score--;
						audio->stopCue(OUCH1);
						audio->stopCue(OUCH2);
						audio->stopCue(SAD);
						switch(rand()%3+1){
						case 1:
							audio->playCue(OUCH1);
							break;
						case 2:
							audio->playCue(OUCH2);
							break;
						case 3:
							audio->playCue(SAD);
						}
					}
				}
			}
		}

		for(int i=0; i<gameNS::NUM_MONEY; i++)
		{
			if(player.collided(&money[i]))
			{
				money[i].setInActive();
				score += money[i].getPoints();
				audio->playCue(CASH);
			}
			money[i].update(dt);
		}
	
		floor.update(dt);
		//gameObject2.update(dt);
		//gameObject3.update(dt);
		//spinner.update(dt);
		xLine.update(dt);
		//xLine.setPosition(player.getPosition());
		yLine.update(dt);
		//yLine.setPosition(player.getPosition());
		zLine.update(dt);
		//zLine.setPosition(player.getPosition());
		//wall.update(dt);
		//quad1.update(dt);

		for(int i=0; i<gameNS::NUM_WALLS; i++)walls[i].update(dt);
		for(int i=0; i<gameNS::NUM_CAMS; i++)
		{
			for (int j = 0; j < pBullets.size(); j++) {
				if(pBullets[j]->collided(&enemyCam[i])&& enemyCam[i].getActiveState())
				{
					enemyCam[i].setInActive();
					audio->playCue(HIT);
					pBullets[j]->setInActive();
					pBullets[j]->setVelocity(D3DXVECTOR3(0,0,0));
					pBullets[j]->setPosition(D3DXVECTOR3(0,0,0));
					score++;
				}
			}
			enemyCam[i].update(dt, &player);
			enemyCam[i].shoot(&player);
			//if(!enemyCam[i].getActiveState())
			//	enemyTimer[i] = 0;
			//for(int k=0; k<enBullets[k].size(); k++)
			//{
			//	enBullets[i][k]->update(dt);
			//}
		}
		int numberInRange=0;
		for(int i=0; i<gameNS::NUM_CAMS; i++){
			if(enemyCam[i].isInRange(player.getPosition()) && enemyTimer[i] == 0 && enemyCam[i].getActiveState()){
				audio->playCue(WOOP_WOOP);
				enemyTimer[i]=1;
			}
			if(!enemyCam[i].isInRange(player.getPosition()) || !enemyCam[i].getActiveState()){
				enemyTimer[i]=0;
			}
			numberInRange+=enemyTimer[i];
		}
		if(numberInRange==0) audio->stopCue(WOOP_WOOP);
		//spinAmount += dt ;
		//if (ToRadian(spinAmount*40)>2*PI)
		//	spinAmount = 0;
		//Build the view matrix.
		//D3DXVECTOR3 pos(-100.0f,100.0f,50.0f);
		//D3DXVECTOR3 pos(-50.0f, 150.0f, 0.0f);
		//D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	}
	else if (startScreen)
	{
		if(input->anyKeyPressed()) startScreen = false;
	}
	else
	{
		Sleep(1000);
		if(input->anyKeyPressed())
		{
			endScreen = false;
			PostQuitMessage(0);
		}
	}
	D3DXVECTOR3 pos(player.getPosition().x - 25, player.getPosition().y + 50, player.getPosition().z);
	D3DXVECTOR3 target(player.getPosition());
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

#pragma region first-pass cleanup
	//For the first update pass, we want to remove any money that is colliding with cameras or walls
	if(firstpass)
	{
		firstpass = false;
		for(int i=0; i<gameNS::NUM_WALLS; i++)
		{
			for(int k=0; k<gameNS::NUM_MONEY; k++)
			{
				if(money[k].getActiveState() && money[k].collided(&walls[i]))
				{
					money[k].setInActive();
				}
			}
		}
		for(int i=0; i<gameNS::NUM_CAMS; i++)
		{
			for(int k=0; k<gameNS::NUM_MONEY; k++)
			{
				if(money[k].getActiveState() && money[k].collided(&enemyCam[i]))
				{
					money[k].setInActive();
				}
			}
		}
	}
#pragma endregion
}

void ColoredCubeApp::drawScene()
{
	D3DApp::drawScene();
	incrementedYMargin = 5;
	int lineHeight = 20;
	if(!startScreen && !endScreen)
	{
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
		for (int i = 0; i < ragePickups.size(); i++)
			ragePickups[i].draw(mfxWVPVar, mTech, &mVP);

		//setting the color flip variable in the shader
		mfxFLIPVar->SetRawValue(&foo[0], 0, sizeof(int));

		//draw the lines
		//drawLine(&xLine);
		//drawLine(&yLine);
		//drawLine(&zLine);

	
		/*****************************************
		Walls!
		*******************************************/
		for(int i=0; i<gameNS::NUM_WALLS; i++)walls[i].draw(mfxWVPVar, mTech, &mVP);
		for(int i=0; i<gameNS::NUM_MONEY; i++) money[i].draw(mfxWVPVar, mTech, &mVP);
	
		////draw the boxes
		//test.draw(mfxWVPVar, mTech, &mVP);
		floor.draw(mfxWVPVar, mTech, &mVP);
		player.draw(mfxWVPVar, mTech, &mVP);
		//pBullet.draw(mfxWVPVar, mTech, &mVP);
		//Player & bullet classes implemented
		//gravball.draw(mfxWVPVar, mTech, &mVP);
		//gameObject1.draw(mfxWVPVar, mTech, &mVP);	

		/*****************************************
		Enemy Cameras
		*******************************************/
		for(int i=0; i<gameNS::NUM_CAMS; i++){
			enemyCam[i].draw(mfxWVPVar, mTech, &mVP);
	//		enBullet[i].draw(mfxWVPVar, mTech, &mVP);
			for(int j=0; j<enBullets[i].size(); j++)
			{
				enBullets[i][j]->draw(mfxWVPVar, mTech, &mVP);
			}
		}

		// We specify DT_NOCLIP, so we do not care about width/height of the rect.
		RECT R = {5, 5, 0, 0};
		//mFont->DrawText(0, mFrameStats.c_str(), -1, &R, DT_NOCLIP, BLACK);
		for (int i = 0; i < debugText.getSize(); i++)
		{
			int xMargin = debugText.lines[i].x;
			int yMargin = debugText.lines[i].y;

			if (xMargin == -1)
				xMargin = 5;
			if (yMargin == -1) {
				yMargin = incrementedYMargin;
				incrementedYMargin += lineHeight;
			}

			
		}
		//RECT POS = {xMargin, yMargin, 0, 0};
		RECT POS = {5, 5, 0, 0};

		std::wostringstream outs;   
		outs.precision(6);
		//outs << debugText.lines[i].s.c_str();
		outs << L"Score: " << score;
		std::wstring sc = outs.str();

		mFont->DrawText(0, sc.c_str(), -1, &POS, DT_NOCLIP, WHITE);
	}
	else if(startScreen)
	{		
		for (int i = 0; i < sText.getSize(); i++)
		{
			int xMargin = sText.lines[i].x;
			int yMargin = sText.lines[i].y;

			if (xMargin == -1)
				xMargin = 5;
			if (yMargin == -1) {
				yMargin = incrementedYMargin;
				incrementedYMargin += lineHeight;
			}

			RECT POS = {xMargin, yMargin, 0, 0};

			std::wostringstream outs;   
			outs.precision(6);
			outs << sText.lines[i].s.c_str();

			mFont->DrawText(0, outs.str().c_str(), -1, &POS, DT_NOCLIP, WHITE);
		}
	}
	else
	{
		for (int i = 0; i < eText.getSize(); i++)
		{
			int xMargin = eText.lines[i].x;
			int yMargin = eText.lines[i].y;

			if (xMargin == -1)
				xMargin = 5;
			if (yMargin == -1) {
				yMargin = incrementedYMargin;
				incrementedYMargin += lineHeight;
			}

			RECT POS = {xMargin, yMargin, 0, 0};

			std::wostringstream outs;   
			outs.precision(6);
			outs << eText.lines[i].s.c_str();

			mFont->DrawText(0, outs.str().c_str(), -1, &POS, DT_NOCLIP, WHITE);
		}
		//RECT POS = {xMargin, yMargin, 0, 0};
		RECT POS = {300, 350, 0, 0};

		std::wostringstream outs;
		outs.precision(6);
		//outs << debugText.lines[i].s.c_str();
		outs << L"Score: " << score;
		std::wstring sc = outs.str();

		mFont->DrawText(0, sc.c_str(), -1, &POS, DT_NOCLIP, WHITE);
	}
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