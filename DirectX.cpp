#include "DirectX.h"
#include "adrawing.hpp"
#include "amenu.hpp"
#include "aconsole.hpp"

IDirect3D9Ex* p_Object = 0;
IDirect3DDevice9Ex* p_Device = 0;
D3DPRESENT_PARAMETERS p_Params;
RECT rc;


int DirectXInit(HWND hWnd)
{
	if(FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(1);

	ZeroMemory(&p_Params, sizeof(p_Params));    
    p_Params.Windowed = TRUE;   
    p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;    
    p_Params.hDeviceWindow = hWnd;    
	p_Params.MultiSampleQuality   = D3DMULTISAMPLE_NONE;
    p_Params.BackBufferFormat = D3DFMT_A8R8G8B8 ;     
    p_Params.BackBufferWidth = Width;    
    p_Params.BackBufferHeight = Height;    
    p_Params.EnableAutoDepthStencil = TRUE;
    p_Params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		exit(1);

	return 0;
}


int Render()
{
	if (tWnd == GetForegroundWindow())
	{
		char * value = tWindowName;

		HWND newhwnd = FindWindow(NULL, value);
		if (newhwnd != NULL) {
			GetWindowRect(newhwnd, &rc);
		}
		else {
			ExitProcess(0);
		}

		p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
		p_Device->BeginScene();


		//Drawing Stuff old
		/*
		ID3DXFont* pFont;
		D3DXCreateFont(p_Device, 50, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);
		DrawString("Simple Text", 10, 10, 255, 255, 0, 0, pFont);
		*/

		// Drawing agui
		adrawing::initDrawing(p_Device);
		amenu::drawMenu();
		aconsole::drawConsole();

		static int counter = 0; char tempStr[50];
		sprintf_s(tempStr, ARRAYSIZE(tempStr), "I'm the counter 0x%p, %i", &counter, counter++);

		aconsole::printToWatchList(0, "I'm in a Loop");
		aconsole::printToWatchList(1, tempStr);

		if (counter < 20)
			aconsole::printToConsole(tempStr);


		p_Device->EndScene();
		p_Device->PresentEx(0, 0, 0, 0, 0);
		return 0;
	}
}