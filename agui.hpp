#pragma once
#include <vector>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <Windows.h>
//#include <d3dx9.h>
#include "adrawing.hpp"


namespace agui {

	
	enum controlType {
		WINDOW,
		TITLE,
		RIBBON,
		TAB,
		FRAME,
		BUTTON,
		SLIDER,
		CHECKBOX,
		HOTKEY,
		LABEL,
		CONSOLELABEL,
		TEXTBOX,
		COMBOBOX
	};

	struct guiFunc;
	struct window;
	struct titlebar;
	struct ribbon;
	struct tab;
	struct frame;

	struct guiObj {

		window* win;
		guiObj* up;
		controlType		cType;

		int				xPos;
		int				yPos;
		int				width;
		int				height;

		char*			caption;
		int				id;
		//bool			mouseOver;

		bool			bigFont;
		MY_D3DRECT		rect;
		MY_D3DRECT		mRect;

		guiObj(window* winPtr, guiObj* upPtr, controlType ct, char* srcCaption, int x, int y, int w, int h);
		void styleUpdate(bool _bigFont = false);
		virtual void drawBackground();
		virtual void drawBorder();
		virtual void drawStr();
		virtual void drawExtra();
		virtual void draw();
		virtual void updateRectPosition() = 0;
		virtual void functionUpdate();
	};

	struct guiFunc {

		window*		activeWindow;
		titlebar*	activeTitlebar;
		ribbon*		activeRibbon;
		tab*		activeTab;
		frame*		activeFrame;

		int			gObjCounter;

		bool		setStartTab;

		guiFunc();
		~guiFunc();
		void init();
		void drawAll();
		int getTabCount(guiObj* desTab, int& pos);
		void updateInFramePos(guiObj* o);
	};
	inline guiFunc* gFunc = nullptr;

	void setGuiFunc(guiFunc* newGuiFunc);

	struct window : public guiObj {
		std::vector<guiObj*> pGuiObj;
		bool* active;
		int*  myKey;

		window(char* Caption, bool* show, int* key, int xPos = 100, int yPos = 100, int width = 480, int height = 270);
		void updateRectPosition() override;
		void functionUpdate() override;
	};

	struct titlebar : public guiObj {

		bool onMoving;
		POINT mousePos;
		MY_D3DRECT oldWinPos;
		
		titlebar(char* Caption, window* wPtr = gFunc->activeWindow, int height = 25);
		void updateRectPosition() override;
		void drawBackground() override;
		void moveWindow();
	};

	struct ribbon : public guiObj {
		std::vector<guiObj*> ribbonObj;

		ribbon(char* Caption, window* wPtr = gFunc->activeWindow, int xPos = 0, int yPos = 10, int width = 480, int height = 40);
		void updateRectPosition() override;
	};

	struct tab : public guiObj {
		bool active = true;

		tab(char* Caption, window* wPtr = gFunc->activeWindow, ribbon* rPtr = gFunc->activeRibbon);
		void updateRectPosition() override;
		void drawBackground() override;
		void drawBorder() override;
		void functionUpdate() override;
	};

	struct frame : public guiObj {
		std::vector<guiObj*> frameObj;

		frame(char* Caption, window* wPtr = gFunc->activeWindow, tab* tPtr = gFunc->activeTab);
		void updateRectPosition() override;
		void drawStr() override;
	};

	struct button : public guiObj {
		bool* btnActive;
		unsigned long lastTick;

		button(char* Caption, bool* btn, window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawExtra() override;
		void drawButton(const MY_D3DRECT& rect);
		void drawStr() override;
		void functionUpdate() override;
	};
	struct slider : public guiObj {

		int* actual;
		int maximal;
		bool onMoving;
		POINT oldMousePos;
		POINT newMousePos;

		slider(char* Caption, int* is, int max, window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawExtra() override;
		void drawSlider(const MY_D3DRECT& sldRect);
		void drawStr() override;
		void functionUpdate() override;
	};
	struct checkbox : public guiObj {

		bool* active;
		checkbox(char* Caption, bool* isActive, window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawExtra() override;
		void drawCheckbox(const MY_D3DRECT& rect);
		void drawStr() override;
		void functionUpdate() override;
	};

	struct hotkey : public guiObj {

		int* myKey;
		int onSearch;
		unsigned long lastTick;

		hotkey(char* Caption, int* key, window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawExtra() override;
		void drawHotkey(const MY_D3DRECT& hkyRect);
		void drawStr() override;
		void functionUpdate() override;
	};

	struct label : public guiObj {

		label(char* Caption, window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawStr() override;
	};

	struct consoleLabel : public guiObj {

		char** ptrPtrStr;

		consoleLabel(char* Caption, char** ppStr, window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawStr() override;
	};

	struct textbox : public guiObj {

		bool onInput;
		int charCounter;
		int lastKey;
		int newKey;
		unsigned long lastTick;

		textbox(char* Caption, window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawExtra() override;
		void drawStr() override;
		void functionUpdate() override;
	};

	struct combobox : public guiObj {

		int maximal;
		char** str;
		int* actual;
		bool isOpen;

		combobox(char* Caption, int* selected, int argc, char* argv[], window* wPtr = gFunc->activeWindow, frame* fPtr = gFunc->activeFrame);
		void updateRectPosition() override;
		void drawExtra() override;
		void drawMenu();
		void drawStr() override;
		void functionUpdate() override;
	};
}