#include "ainput.hpp"

namespace ainput {

#define highestBit 0x8000
#define lowestBit  0x0001


	POINT getMousePos() {
		static bool init = false;
		HWND newhwnd = GetForegroundWindow();
		POINT cursor_pos;

		GetCursorPos(&cursor_pos);
		ScreenToClient(newhwnd, &cursor_pos);
		return cursor_pos;
	}

	bool isMouseInRect(_D3DRECT& rect)
	{
		POINT p = getMousePos();
		if (p.x > rect.x1 && p.x < rect.x2 && p.y > rect.y1 && p.y < rect.y2)
			return true;
		return false;
	}

	bool isKeyClicked(int vKey) {

		static bool selectedKeyPress[255]{ false };
		if (vKey > 255)
			return false;

		int selectedKey = GetAsyncKeyState(vKey);
		if (selectedKey & highestBit && selectedKeyPress[vKey] == false) {
			selectedKeyPress[vKey] = true;
			return true;
		}
		if (!(selectedKey & highestBit) && selectedKeyPress[vKey] == true) {
			selectedKeyPress[vKey] = false;	
		}
		return false;
	}

	bool isKeyDown(int vKey) {

		if (vKey > 255)
			return false;

		int selectedKey = GetAsyncKeyState(vKey);
		if (selectedKey & highestBit) {
			return true;
		}
		return false;
	}

	int getPressedKey()
	{

		for (int i = 0; i < 255; i++) {
			int temp = GetAsyncKeyState(i);
			if (temp & highestBit)
				return i;
		}
		return 0;
	}

	bool isMouseClicked() {
		return isKeyClicked(VK_LBUTTON);
	}

	bool isMouseDown() {
		return isKeyDown(VK_LBUTTON);
	}

}