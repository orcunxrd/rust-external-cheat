#include "direct3d.h"
#include "main\ui.h"
//Direct3D Resources
IDirect3DDevice9Ex* pDevice;
IDirect3D9Ex* pInstance;
D3DPRESENT_PARAMETERS pParams;

ID3DXFont* pFontVisualsLarge;
ID3DXFont* pFontVisualsSmall;

ID3DXFont* pFontUILarge;
ID3DXFont* pFontUISmall;

ID3DXLine* pLine;

char buf[128];

//Overlay FPS
float flCurTick, flOldTick;
int iTempFPS, iRenderFPS;

static bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	PDIRECT3DTEXTURE9 texture;

	HRESULT hr = D3DXCreateTextureFromFileA(pDevice, filename, &texture);
	if (hr != S_OK)
		return false;

	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

PDIRECT3DTEXTURE9 my_texture;
PDIRECT3DTEXTURE9 my_texture2;
#include "globals.h"
ImFont* main_font;

BOOL D3DInitialize(HWND hWnd) {
	ImGuiIO& io = ImGui::GetIO();
	
	Direct3DCreate9Ex(D3D_SDK_VERSION, &pInstance);
	pParams.Windowed = TRUE;
	pParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pParams.hDeviceWindow = hWnd;
	pParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	pParams.BackBufferWidth = Globals::rWidth;
	pParams.BackBufferHeight = Globals::rHeight;
	pParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	pParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pParams.EnableAutoDepthStencil = TRUE;
	pParams.AutoDepthStencilFormat = D3DFMT_D16;
	pInstance->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pParams, 0, &pDevice);
	
	if (pDevice == NULL) { return FALSE; }

	//ImGui
	ImGui::CreateContext();

	ImFontConfig fontCfg = ImFontConfig();
	io.DeltaTime = 1.0f / 60.0f;
	fontCfg.RasterizerFlags = 0x01;
	fontCfg.OversampleH = fontCfg.OversampleV = 1;
	fontCfg.PixelSnapH = true;

	ImGui_ImplDX9_Init(hWnd, pDevice);
	return TRUE;
}

void D3DRender() {
	pParams.BackBufferHeight = Globals::rHeight;
	pParams.BackBufferWidth = Globals::rWidth;
	pDevice->Reset(&pParams);
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	pDevice->BeginScene();

	if (Globals::tWnd == GetForegroundWindow() || GetActiveWindow() == GetForegroundWindow())
	{
		iGetFPS(&Globals::iFPS);
		m_render->Draw_Render_Menu(pDevice);
	}

	pDevice->EndScene();
	pDevice->PresentEx(0, 0, 0, 0, 0);
}



void iGetFPS(int* fps) {

	flCurTick = clock() * 0.001f;
	iTempFPS++;
	if ((flCurTick - flOldTick) > 1.0f) {
		iRenderFPS = iTempFPS;

		iTempFPS = 0;
		flOldTick = flCurTick;
	}

	*(int*)fps = iRenderFPS;
}