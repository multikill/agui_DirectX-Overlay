#pragma once
#include <Windows.h>
#include <d3dx9.h>

namespace ainput {

	bool isKeyClicked(int vKey);
	bool isKeyDown(int vKey);
	int  getPressedKey();

	bool isMouseClicked();
	bool isMouseDown();
	POINT getMousePos();
	bool isMouseInRect(_D3DRECT& rect);

}