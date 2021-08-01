#pragma once

#include "mainrenderer.h"


extern ID3DXLine* pLine;

extern ID3DXFont* pFontVisualsLarge;
extern ID3DXFont* pFontVisualsSmall;

extern ID3DXFont* pFontUILarge;
extern ID3DXFont* pFontUISmall;

void D3DRender();
BOOL D3DInitialize(HWND hWnd);

void iGetFPS(int* fps);