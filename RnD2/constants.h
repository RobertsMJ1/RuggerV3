#ifndef Constants_H
#define Constants_H
#include <windows.h>

// window
const char CLASS_NAME[] = "RUGBOI";
const char GAME_TITLE[] = "Rugger & Dunstan II: Rugger Resumes";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1280;               // width of game in pixels
const UINT GAME_HEIGHT = 720;               // height of game in pixels

//Defines to make life easier
#define Vector3		D3DXVECTOR3
#define Matrix		D3DXMATRIX
#define Identity	D3DXMatrixIdentity
#define Translate	D3DXMatrixTranslation
#define RotateX		D3DXMatrixRotationX
#define RotateY		D3DXMatrixRotationY
#define RotateZ		D3DXMatrixRotationZ
#define ToRadian	D3DXToRadian
#define ToDegree	D3DXToDegree
#define Scale	D3DXMatrixScaling
#define Normalize D3DXVec3Normalize
#define MultiplyMatrix D3DXMatrixMultiply
//const double PI = 3.14159265;
const double GRAVITY = 2.67428e-11f;

const UCHAR KEY_A	= 'A';
const UCHAR KEY_D   = 'D';
const UCHAR KEY_W	= 'W';
const UCHAR KEY_S   = 'S';

#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define LP_DXFONT   LPD3DX10FONT


//**********AUDIO**********//

// WAVE_BANK_1 must be location of .xwb file.
wchar_t const WAVE_BANK[]  = L"audio\\Win\\Wave Bank.xwb";
// SOUND_BANK_1 must be location of .xsb file.
wchar_t const SOUND_BANK[] = L"audio\\Win\\Sound Bank.xsb";

//audio cues
const char PLAYER_FIRE[] = "oomph";
const char HIT[] = "Alien Death Ray";
const char WOOP_WOOP[] = "Woop Woop";

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility


#endif