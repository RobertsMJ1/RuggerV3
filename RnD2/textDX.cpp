// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textDX.cpp v1.1
// DirectX font

#include "textDX.h"

//=============================================================================
// default constructor
//=============================================================================
TextDX::TextDX()
{
    color = SETCOLOR_ARGB(255,255,255,255); // default to white font

    // set font position
    fontRect.top = 0;
    fontRect.left = 0;
    fontRect.right = GAME_WIDTH;
    fontRect.bottom = GAME_HEIGHT;
    dxFont = NULL;
    angle  = 0;
}

//=============================================================================
// destructor
//=============================================================================
TextDX::~TextDX()
{
    safeRelease(dxFont);
}

//=============================================================================
// Create DirectX Font
//=============================================================================
bool TextDX::initialize(D3DApp* d, int height, bool bold, bool italic, 
                        const std::string* fontName)
{
    //graphics = g;                   // the graphics system
	d3dApp = d;
    UINT weight = FW_NORMAL;
    if(bold)
        weight = FW_BOLD;

    // create DirectX font
    /*if(FAILED(D3DX10CreateFont(d3dApp->getDevice(), height, 0, weight, 1, italic,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(),
        &dxFont))) return false;*/
	
	LPCTSTR fontname = reinterpret_cast<LPCTSTR>(fontName);
	D3DX10CreateFont(d3dApp->getDevice(), height, 0, weight, 1, true, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, reinterpret_cast<LPCWSTR>(fontName), &dxFont);
    // Create the tranformation matrix
    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);

    return true;
}

//=============================================================================
// Print text at x,y 
// Return 0 on fail, height of text on success
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, int x, int y)
{
    if(dxFont == NULL)
        return 0;
    // set font position
    fontRect.top = y;
    fontRect.left = x;

    // Rotation center
    D3DXVECTOR2 rCenter=D3DXVECTOR2((float)x,(float)y);
    // Setup matrix to rotate text by angle
    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, &rCenter, angle, NULL);
    // Tell the sprite about the matrix "Hello Neo"
    d3dApp->getSprite()->SetTransform(&matrix);
    return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &fontRect, DT_LEFT, color);
}

//=============================================================================
// Print text inside RECT using DirectX text format
// Return 0 on fail, height of text on success
// pre: spriteBegin()
// post: spriteEnd()
//=============================================================================
int TextDX::print(const std::string &str, RECT &rect, UINT format)
{
    if(dxFont == NULL)
        return 0;

    // Setup matrix to not rotate text
    D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
    // Tell the sprite about the matrix "Hello Neo"
    graphics->getSprite()->SetTransform(&matrix);
    return dxFont->DrawTextA(graphics->getSprite(), str.c_str(), -1, &rect, format, color);
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void TextDX::onLostDevice()
{
    if(dxFont == NULL)
        return;
    dxFont->OnLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void TextDX::onResetDevice()
{
    if(dxFont == NULL)
        return;
    dxFont->OnResetDevice();
}
